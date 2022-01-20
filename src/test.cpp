/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2022-01-20 19:50:32
 * @LastEditors: sueRimn
 * @LastEditTime: 2022-01-20 21:27:30
 */
// /*
//  * @Descripttion: 
//  * @version: 
//  * @Author: sueRimn
//  * @Date: 2022-01-20 19:50:32
//  * @LastEditors: sueRimn
//  * @LastEditTime: 2022-01-20 20:06:42
//  */
// #include "espconn.h"

// #include "mem.h"
// struct espconn user_udp_espconn;
// static os_timer_t test_timer;

// sint8 espconn_send(struct espconn *espconn,uint8 *psent,uint16 length);

// void ICACHE_FLASH_ATTR user_udp_recv_cb(void *arg,char *pdata,unsigned short len){
//  os_printf("udp have received data:%s\r\n",pdata);
// }
// void ICACHE_FLASH_ATTR user_udp_send(void){
//          uint8 hwaddr[6];
//          char DeviceBuffer[40]={0};
//          wifi_get_macaddr(STATION_IF,hwaddr);
//         //  os_sprintf(DeviceBuffer,"DEVICE MAC ADDRESS IS:"MACSTR"!!\r\nESP8266 IOT!\r\n",MAC2STR(hwaddr));
//          espconn_send(&user_udp_espconn,(uint8 *)DeviceBuffer,os_strlen(DeviceBuffer));

// }

// void ICACHE_FLASH_ATTR user_udp_sent_cb(void *arg){
//          os_printf("SEND SUCCESS!\r\n");
//          os_timer_disarm(&test_timer);
//          os_timer_setfn(&test_timer,(os_timer_func_t *)user_udp_send,0);
//          os_timer_arm(&test_timer,1000,0);
// }
// void s(){
//     user_udp_espconn.type=ESPCONN_UDP; //设置为UDP通信
//     //开辟UDP参数需要的空间
//     user_udp_espconn.proto.udp=(esp_udp *)os_zalloc(sizeof(esp_udp));
//     //设置本地端口和远程端口
//     user_udp_espconn.proto.udp->local_port=2525;
//     user_udp_espconn.proto.udp->remote_port=1024;
//     //设置远程IP
//     const char udp_remote_ip[4]={255,255,255,255};
//     os_memcpy(user_udp_espconn.proto.udp->remote_ip,udp_remote_ip,4);
//     //设置发送完成和接收完成的回调函数
//     espconn_regist_recvcb(&user_udp_espconn,user_udp_recv_cb);
//     espconn_regist_sentcb(&user_udp_espconn,user_udp_sent_cb);
//     //使UDP参数生效
//     espconn_create(&user_udp_espconn);
//     //UDP发送函数
//     user_udp_send();
// }

