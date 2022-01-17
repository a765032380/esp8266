#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <PubSubClientTools.h>
#include <ArduinoJson.h>
#include <Servo.h>

#include <Thread.h>             // https://github.com/ivanseidel/ArduinoThread
#include <ThreadController.h>
#include "LittleFS.h"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "LittleFS.h"
#include <airkiss.h>

// #include "gllwifi.cpp"

#define MQTT_SERVER "gll.pub"

WiFiClient espClient;
PubSubClient pubSubclient(MQTT_SERVER, 1883, espClient);
PubSubClientTools mqtt(pubSubclient);

ThreadController threadControl = ThreadController();
Thread thread = Thread();

int value = 0;
const String s = "";

const String testtopic = "lottopic_zyf";

Servo servo_16;   //舵机输出口 GIPO16
bool oState = false;
bool oSubscribe = false;

#define LED 2

struct GLLWifiConfig {
  String SSID;
  String Passwd;
  String Server = "esaylinker";
  String Token = "0000";
};
GLLWifiConfig _gllWifiConfig;


bool gllWifiSaveConfig() {
  DynamicJsonDocument doc(1024);
  JsonObject root = doc.to<JsonObject>();
  root["SSID"] = _gllWifiConfig.SSID;
  root["Passwd"] = _gllWifiConfig.Passwd;
  root["Server"] = _gllWifiConfig.Server;
  root["Token"] = _gllWifiConfig.Token;

  File configFile = LittleFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }
  if (serializeJson(doc, configFile) == 0) {
    Serial.println("Failed to write to file");
    return false;
  }
  return true;
}
void gllWifiSmartConfig()
{
  Serial.println("Use smart config to connect wifi.");
  WiFi.mode(WIFI_STA);
  WiFi.beginSmartConfig();
  while (1)
  {
    Serial.println("Wait to connect wifi...");
    digitalWrite(LED, LOW);
    delay(1000);
    digitalWrite(LED, HIGH);
    delay(1000);
    if (WiFi.smartConfigDone())
    {
      Serial.println("WiFi connected by smart config.");
      Serial.print("SSID:");
      Serial.println(WiFi.SSID());
      Serial.print("IP Address:");
      Serial.println(WiFi.localIP());
      _gllWifiConfig.SSID = WiFi.SSID();
      _gllWifiConfig.Passwd = WiFi.psk();
      if (!gllWifiSaveConfig()) {
        Serial.println("Failed to save config");
      } else {
        Serial.println("Config saved");
      }

      break;
    }
  }
}
void gllWifiBaseConfig() {
  Serial.println("Use base config to connect wifi.");
  WiFi.mode(WIFI_STA);
  WiFi.begin(_gllWifiConfig.SSID, _gllWifiConfig.Passwd);
  //连接超时时间，30s后没有连接将会转入SmartConfig
  int timeout = 30000;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wait to connect wifi...");
    digitalWrite(LED, LOW);
    delay(150);
    digitalWrite(LED, HIGH);
    delay(150);
    timeout = timeout - 300;
    if (timeout <= 0) {
      Serial.println("Wifi connect timeout, use smart config to connect...");
      gllWifiSmartConfig();
      return;
    }
  }
  Serial.println("WiFi connected by base config.");
  Serial.print("SSID:");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address:");
  Serial.println(WiFi.localIP());
}

bool LoadConfig() {
  File configFile = LittleFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }
  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, configFile);
  if (error) {
    Serial.println("Failed to read file, using default configuration");
    return false;
  }
  _gllWifiConfig.SSID = doc["SSID"] | "fail";
  _gllWifiConfig.Passwd = doc["Passwd"] | "fail";
  if (_gllWifiConfig.SSID == "fail" || _gllWifiConfig.Passwd == "fail") {
    return false;
  }
  else {
    Serial.println("Load wifi config from spiffs successful.");
    Serial.print("Loaded ssid: ");
    Serial.println(_gllWifiConfig.SSID);
    Serial.print("Loaded passwd: ");
    Serial.println(_gllWifiConfig.Passwd);
    return true;
  }
}

void ConnectServer() {
  if (LoadConfig()) {
        gllWifiBaseConfig();
  } else {
        gllWifiSmartConfig();
  }
  digitalWrite(LED, LOW);
}

bool WiFiWatchDog(){
  if(WiFi.status() != WL_CONNECTED){
    gllWifiBaseConfig();
  }
  return true;
}


void write(int start,int end){
    Serial.println(s + "start:"+ start + "end:" + end);
    servo_16.write(start);  //舵机旋转角度 具体参数需要结合自身情况修改
    delay(1);           //给一个小延时
    delay(1000);   //舵机动作后延时一会回到初始位置
    servo_16.write(end); //设置该角度方便手动开关灯 就是开完灯舵机归位、
}

void writeEnd(int end){
    servo_16.write(end); //设置该角度方便手动开关灯 就是开完灯舵机归位
}

void json(String message){
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, message);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  int code = doc["code"];
  int start = doc["start"];
  int end = doc["end"];
  
  Serial.println(code);
  Serial.println(start);
  Serial.println(end);
  switch (code)
  {
  case 0:
    writeEnd(end);
    break;
  case 1:
    write(start,end);
    break;
  default:
    break;
  }
}
void topic_subscriber(String topic, String message) {
  Serial.println(s + "Message arrived in function 3 [" + topic + "] " + message);
  if(topic == testtopic+"/5"){
    return;
  }
  json(message);
}
void publisher() {
  if (pubSubclient.state() != MQTT_CONNECTED){
    if (pubSubclient.connect("ESP8266Client")) {
        Serial.println("mqtt connect success");
        if (!oSubscribe)
        {
          mqtt.subscribe("lottopic_zyf/#",topic_subscriber);
          oSubscribe = true;
        }
        
      } else {
        Serial.println(s + "failed, rc=" + pubSubclient.state());
      }
  }
}

void setup() {
  Serial.begin(115200);
  // WiFiManager wm;

  // 重置设置-为测试擦除存储的凭证
  // 它们由esp库存储
  // wm.resetSettings();
  pinMode(LED, OUTPUT);
  if (!LittleFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }
  //Connect WIFI
  ConnectServer();
  
  Serial.print("IP Address:");
  Serial.println(WiFi.localIP());

  //使用保存的凭证自动连接，
  //然后进入一个阻塞循环等待配置，并返回成功结果

  // Connect to MQTT
  Serial.print(s + "Connecting to MQTT: " + MQTT_SERVER + " ... ");
  if (pubSubclient.connect("ESP8266Client")) {
    Serial.println("connected");
    mqtt.subscribe("lottopic_zyf/#",topic_subscriber);
    Serial.println("mqtt connect success");
  } else {
    Serial.println(s + "failed, rc=" + pubSubclient.state());
  }

  // Enable Thread
  thread.onRun(publisher);
  thread.setInterval(3000);
  threadControl.add(&thread);

  servo_16.attach(16);
}

void loop() {
  WiFiWatchDog();
  // 把你的主代码放在这里，重复运行:
  pubSubclient.loop();
  threadControl.run();

}
