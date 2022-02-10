
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <PubSubClient.h>
#include <PubSubClientTools.h>
#include <ArduinoJson.h>
#include <Thread.h>             // https://github.com/ivanseidel/ArduinoThread
#include <ThreadController.h>
#include "gllServo.h"


#define MQTT_SERVER "gll.pub"

const String testtopic = "lottopic_gll";

const char* client_id = "ESP8266Client_GLL"; 

const String s = "";

bool oSubscribe = false;

WiFiClient espClient;
PubSubClient pubSubclient(MQTT_SERVER, 1883, espClient);
PubSubClientTools mqtt(pubSubclient);


ThreadController threadControl = ThreadController();
Thread thread = Thread();


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
  // user_udp_send();
  if (pubSubclient.state() != MQTT_CONNECTED){
    if (pubSubclient.connect(client_id)) {
        Serial.println("mqtt connect success");
        if (!oSubscribe)
        {
          mqtt.subscribe(testtopic+"/#",topic_subscriber);
          oSubscribe = true;
        }
        
      } else {
        Serial.println(s + "failed, rc=" + pubSubclient.state());
      }
  }
}

void gll_mqtt_loop(){
  pubSubclient.loop();
  threadControl.run();
}
void gll_mqtt_setup(){
    // Connect to MQTT
  Serial.print(s + "Connecting to MQTT: " + MQTT_SERVER + " ... ");
  if (pubSubclient.connect(client_id)) {
    Serial.println("connected");
    mqtt.subscribe(testtopic+"/#",topic_subscriber);
    Serial.println("mqtt connect success");
  } else {
    Serial.println(s + "failed, rc=" + pubSubclient.state());
  }

  attach(16);
  // Enable Thread
  thread.onRun(publisher);
  thread.setInterval(1000);
  threadControl.add(&thread);
}