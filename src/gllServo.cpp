/**
 * @file gllServo.cpp
 * @author your name (you@domain.com)
 * @brief  舵机控制
 * @version 0.1
 * @date 2022-01-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <Servo.h>

Servo servo_16;   //舵机输出口 GIPO16

const String s = "";


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

void attach(int servoId){
    servo_16.attach(servoId);
}