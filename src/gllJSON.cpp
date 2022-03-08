/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2022-01-20 12:23:03
 * @LastEditors: sueRimn
 * @LastEditTime: 2022-01-20 12:23:04
 */

#include <ArduinoJson.h>
#include "gllServo.h"

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
  
//   Serial.println(code);
//   Serial.println(start);
//   Serial.println(end);
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
