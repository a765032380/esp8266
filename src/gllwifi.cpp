// #include <ESP8266WiFi.h>
// #include <ArduinoJson.h>
// #include "LittleFS.h"

// #define LED 2

// struct GLLWifiConfig {
//   String SSID;
//   String Passwd;
//   String Server = "esaylinker";
//   String Token = "0000";
// };
// GLLWifiConfig _gllWifiConfig;


// bool gllWifiSaveConfig() {
//   DynamicJsonDocument doc(1024);
//   JsonObject root = doc.to<JsonObject>();
//   root["SSID"] = _gllWifiConfig.SSID;
//   root["Passwd"] = _gllWifiConfig.Passwd;
//   root["Server"] = _gllWifiConfig.Server;
//   root["Token"] = _gllWifiConfig.Token;

//   File configFile = LittleFS.open("/config.json", "w");
//   if (!configFile) {
//     Serial.println("Failed to open config file for writing");
//     return false;
//   }
//   if (serializeJson(doc, configFile) == 0) {
//     Serial.println("Failed to write to file");
//     return false;
//   }
//   return true;
// }
// void gllWifiSmartConfig()
// {
//   Serial.println("Use smart config to connect wifi.");
//   WiFi.mode(WIFI_STA);
//   WiFi.beginSmartConfig();
//   while (1)
//   {
//     Serial.println("Wait to connect wifi...");
//     digitalWrite(LED, LOW);
//     delay(1000);
//     digitalWrite(LED, HIGH);
//     delay(1000);
//     if (WiFi.smartConfigDone())
//     {
//       Serial.println("WiFi connected by smart config.");
//       Serial.print("SSID:");
//       Serial.println(WiFi.SSID());
//       Serial.print("IP Address:");
//       Serial.println(WiFi.localIP());
//       _gllWifiConfig.SSID = WiFi.SSID();
//       _gllWifiConfig.Passwd = WiFi.psk();
//       if (!gllWifiSaveConfig()) {
//         Serial.println("Failed to save config");
//       } else {
//         Serial.println("Config saved");
//       }
//       break;
//     }
//   }
// }
// void gllWifiBaseConfig() {
//   Serial.println("Use base config to connect wifi.");
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(_gllWifiConfig.SSID, _gllWifiConfig.Passwd);
//   //连接超时时间，30s后没有连接将会转入SmartConfig
//   int timeout = 30000;
//   while (WiFi.status() != WL_CONNECTED) {
//     Serial.println("Wait to connect wifi...");
//     digitalWrite(LED, LOW);
//     delay(150);
//     digitalWrite(LED, HIGH);
//     delay(150);
//     timeout = timeout - 300;
//     if (timeout <= 0) {
//       Serial.println("Wifi connect timeout, use smart config to connect...");
//       gllWifiSmartConfig();
//       return;
//     }
//   }
//   Serial.println("WiFi connected by base config.");
//   Serial.print("SSID:");
//   Serial.println(WiFi.SSID());
//   Serial.print("IP Address:");
//   Serial.println(WiFi.localIP());
// }

// bool LoadConfig() {
//   File configFile = LittleFS.open("/config.json", "r");
//   if (!configFile) {
//     Serial.println("Failed to open config file");
//     return false;
//   }
//   StaticJsonDocument<1024> doc;
//   DeserializationError error = deserializeJson(doc, configFile);
//   if (error) {
//     Serial.println("Failed to read file, using default configuration");
//     return false;
//   }
//   _gllWifiConfig.SSID = doc["SSID"] | "fail";
//   _gllWifiConfig.Passwd = doc["Passwd"] | "fail";
//   if (_gllWifiConfig.SSID == "fail" || _gllWifiConfig.Passwd == "fail") {
//     return false;
//   }
//   else {
//     Serial.println("Load wifi config from spiffs successful.");
//     Serial.print("Loaded ssid: ");
//     Serial.println(_gllWifiConfig.SSID);
//     Serial.print("Loaded passwd: ");
//     Serial.println(_gllWifiConfig.Passwd);
//     return true;
//   }
// }

// void ConnectServer() {
//   if (LoadConfig()) {
//         gllWifiBaseConfig();
//   } else {
//         gllWifiSmartConfig();
//   }
//   digitalWrite(LED, LOW);
// }

// bool WiFiWatchDog(){
//   if(WiFi.status() != WL_CONNECTED){
//     gllWifiBaseConfig();
//   }
//   return true;
// }
