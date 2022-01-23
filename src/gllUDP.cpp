#include <WiFiUdp.h>
#include <ESP8266WiFi.h>

unsigned int localUdpPort = 1234;  // 自定义本地监听端口
unsigned int remoteUdpPort = 4321;  // 自定义远程监听端口
char incomingPacket[255];  // 保存Udp工具发过来的消息
char  replyPacket[] = "Hi, this is esp8266\n";  //发送的消息,仅支持英文
WiFiUDP Udp;//实例化WiFiUDP对象

void gll_udp_begin(){
    if(Udp.begin(localUdpPort)){//启动Udp监听服务
    Serial.println("监听成功");
    //打印本地的ip地址，在UDP工具中会使用到
    //WiFi.localIP().toString().c_str()用于将获取的本地IP地址转化为字符串    
    Serial.printf("现在收听IP：%s, UDP端口：%d\n", WiFi.localIP().toString().c_str(), localUdpPort);
  }else{
    Serial.println("监听失败");
  }
}

void gll_udp_loop(){
    //解析Udp数据包
  int packetSize = Udp.parsePacket();//获得解析包
  if (packetSize)//解析包不为空
  {
    //收到Udp数据包
    //Udp.remoteIP().toString().c_str()用于将获取的远端IP地址转化为字符串
    Serial.printf("收到来自远程IP：%s（远程端口：%d）的数据包字节数：%d\n", Udp.remoteIP().toString().c_str(), Udp.remotePort(), packetSize);
      
    // 读取Udp数据包并存放在incomingPacket
    int len = Udp.read(incomingPacket, 255);//返回数据包字节数
    if (len > 0)
    { 
      incomingPacket[len] = 0;//清空缓存
    }
    //向串口打印信息
    Serial.printf("UDP数据包内容为: %s\n", incomingPacket);

    //向udp工具发送消息
    Udp.beginPacket(Udp.remoteIP(), remoteUdpPort);//配置远端ip地址和端口
    Udp.write(replyPacket);//把数据写入发送缓冲区
    Udp.endPacket();//发送数据
  }
}


void gll_udp_send(char packet[]){
    //向udp工具发送消息
    Udp.beginPacket(Udp.remoteIP(), remoteUdpPort);//配置远端ip地址和端口
    Udp.write(packet);//把数据写入发送缓冲区
    Udp.endPacket();//发送数据
}