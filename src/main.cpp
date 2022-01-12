#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <PubSubClientTools.h>
#include <ArduinoJson.h>
#include <Servo.h>

#include <Thread.h>             // https://github.com/ivanseidel/ArduinoThread
#include <ThreadController.h>

#define MQTT_SERVER "gll.pub"

WiFiClient espClient;
PubSubClient client(MQTT_SERVER, 1883, espClient);
PubSubClientTools mqtt(client);

ThreadController threadControl = ThreadController();
Thread thread = Thread();

int value = 0;
const String s = "";

const String testtopic = "lottopic";

Servo servo_16;   //舵机输出口 GIPO16
bool oState = false;

void write(int start,int end){
    servo_16.write(start);  //舵机旋转角度 具体参数需要结合自身情况修改
    delay(1);           //给一个小延时
    delay(1000);   //舵机动作后延时一会回到初始位置
    servo_16.write(end); //设置该角度方便手动开关灯 就是开完灯舵机归位
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
  ++value;
  if (!client.connected()){
    if (client.connect("ESP8266Client")) {
        Serial.println("connected");
        mqtt.subscribe("lottopic/#",topic_subscriber);
      } else {
        Serial.println(s + "failed, rc=" + client.state());
      }
  }
  
}

void setup() {
  WiFi.mode(WIFI_STA); // 显式设置模式，esp默认为STA+AP
  // 这是一个很好的实践，以确保你的代码设置的wifi模式，你想要它。

  // 把你的设置代码放在这里，运行一次:
  Serial.begin(115200);

  //WiFiManager,当地的初始化。一旦它的业务完成，就没有必要再保留它了
  WiFiManager wm;

  // 重置设置-为测试擦除存储的凭证
  // 它们由esp库存储
//  wm.resetSettings();

  //使用保存的凭证自动连接，
  //如果连接失败，它启动一个指定名称的接入点("AutoConnectAP")，
  //如果为空将自动生成SSID，如果密码为空将是匿名AP (wm.autoConnect())
  //然后进入一个阻塞循环等待配置，并返回成功结果

  bool res;
  // res = wm.autoConnect(); // 从chipid自动生成AP名称
   res = wm.autoConnect("AutoConnectAP"); // 匿名 ap

  if (!res) {
    Serial.println("Failed to connect");
    // ESP.restart();
  }
  else {
    //如果你到了这里，你已经连接上了WiFi
    Serial.println("connected...yeey :)");
  }
  // Connect to MQTT
  Serial.print(s + "Connecting to MQTT: " + MQTT_SERVER + " ... ");
  if (client.connect("ESP8266Client")) {
    Serial.println("connected");
    mqtt.subscribe("lottopic/#",topic_subscriber);
  } else {
    Serial.println(s + "failed, rc=" + client.state());
  }

  // Enable Thread
  thread.onRun(publisher);
  thread.setInterval(2000);
  threadControl.add(&thread);
    //这里初始话后没有调用板子自带的led
//    pinMode(LED_BUILTIN, OUTPUT);
    servo_16.attach(16);
}

void loop() {
  // 把你的主代码放在这里，重复运行:
  client.loop();
  threadControl.run();
}
