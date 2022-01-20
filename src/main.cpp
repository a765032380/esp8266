#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <PubSubClientTools.h>
#include <ArduinoJson.h>

#include <Thread.h>             // https://github.com/ivanseidel/ArduinoThread
#include <ThreadController.h>
#include "LittleFS.h"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "LittleFS.h"
#include <airkiss.h>

#include "gllwifi.h"
#include "gllServo.h"
// #include "gllblinker.h"

#define MQTT_SERVER "gll.pub"

WiFiClient espClient;
PubSubClient pubSubclient(MQTT_SERVER, 1883, espClient);
PubSubClientTools mqtt(pubSubclient);

ThreadController threadControl = ThreadController();
Thread thread = Thread();

int value = 0;
const String s = "";

const String testtopic = "lottopic_zyf";

bool oState = false;
bool oSubscribe = false;


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
  // pinMode(LED, OUTPUT);

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

  attach(16);
  // setupBlinker(_gllWifiConfig.SSID.c_str(),_gllWifiConfig.Passwd.c_str());
}

void loop() {
  WiFiWatchDog();
  // 把你的主代码放在这里，重复运行:
  pubSubclient.loop();
  threadControl.run();
  // runBlinker();

}
