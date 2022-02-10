/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2022-01-25 10:35:47
 * @LastEditors: sueRimn
 * @LastEditTime: 2022-02-10 16:24:35
 */

#include "LittleFS.h"
#include <ESP8266WiFi.h>
#include <airkiss.h>

#include "gllwifi.h"
#include "gllUDP.h"
#include "gllMQTT.h"

// #include "gllblinker.h"





void setup() {
  Serial.begin(115200);
  // WiFiManager wm;

  // 重置设置-为测试擦除存储的凭证
  // 它们由esp库存储
  // wm.resetSettings();
  // pinMode(LED, OUTPUT);

  //Connect WIFI
  ConnectServer();
  WiFi.mode(WIFI_STA);
  Serial.print("IP Address:");
  Serial.println(WiFi.localIP());

  //使用保存的凭证自动连接，
  //然后进入一个阻塞循环等待配置，并返回成功结果
  gll_mqtt_setup();
  gll_udp_begin();
  // setupBlinker(_gllWifiConfig.SSID.c_str(),_gllWifiConfig.Passwd.c_str());
}

void loop() {
  WiFiWatchDog();
  // 把你的主代码放在这里，重复运行:
  gll_udp_loop();
  gll_mqtt_loop();
  //获取唯一ID
  ESP.getChipId();
  // runBlinker();

}
