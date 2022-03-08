
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <PubSubClient.h>
#include <PubSubClientTools.h>
#include <Thread.h>             // https://github.com/ivanseidel/ArduinoThread
#include <ThreadController.h>
#include "gllServo.h"
#include "gllJSON.h"
#include <string>
#include<cstdlib>

#define MQTT_SERVER "gll.pub"

const String s = "";

const String testtopic = "lottopic_gll_test";

const String client_id = s+"esp_"+ESP.getChipId();


bool oSubscribe = false;

WiFiClient espClient;
PubSubClient pubSubclient(MQTT_SERVER, 1883, espClient);
PubSubClientTools mqtt(pubSubclient);


ThreadController threadControl = ThreadController();
Thread thread = Thread();

void topic_subscriber(String topic, String message) {
  Serial.println(s + "Message arrived in function 3 [" + topic + "] " + message);
  if(topic == testtopic+"/"+client_id.c_str()){
    return;
  }
  json(message);
}


void publisher() {
  // user_udp_send();
  if (pubSubclient.state() != MQTT_CONNECTED){
    if (pubSubclient.connect(client_id.c_str())) {
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

    Serial.println(s+"mqtt connect client_id="+client_id.c_str());
  if (pubSubclient.connect(client_id.c_str())) {
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