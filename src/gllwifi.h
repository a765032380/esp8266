/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2022-01-19 10:53:33
 * @LastEditors: sueRimn
 * @LastEditTime: 2022-01-19 17:39:38
 */
// struct GLLWifiConfig {
//   String SSID;
//   String Passwd;
//   String Server = "esaylinker";
//   String Token = "0000";
// };

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "LittleFS.h"


bool gllWifiSaveConfig();
void gllWifiSmartConfig();
void gllWifiBaseConfig();
bool LoadConfig();
void ConnectServer();

bool WiFiWatchDog();
