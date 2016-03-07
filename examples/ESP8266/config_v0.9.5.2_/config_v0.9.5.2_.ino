/**
 *ESP8266透传自动设置程序
 *功能：自动设置ESP8266进入透传模式，并重启自动连接贝壳物联平台。
 *By 贝壳物联 2016.1.4
 *贝壳物联：http://www.bigiot.net
 *ESP8266购买地址：https://item.taobao.com/item.htm?id=525028089592
 */
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); //设置模拟串口针脚(RX, TX)
//=============  此处必须修改===================
String WiFiSSID = "XXXX";//填写路由器名称=======
String WiFiPASSWORD = "XXXXXX";//填写WiFi密码===
//==============================================
int flag = 0;
void setup() {
  Serial.begin(9600);
  mySerial.begin(115200);
}
void loop() {
  if(flag == 0){
  Serial.println("setting start");
  //ESP8266通电启动等待
  delay(10000);
  //如果是透传模式，退出透传
  Serial.println("exit pass-through mode");
  mySerial.print("+++");
  delay(1000);
  mySerial.print("AT\r\n");
  delay(1000);
  printmssage();
  //关闭命令回显
  Serial.println("Close command echo");
  mySerial.print("ATE0\r\n");
  delay(1000);
  printmssage();
  //设置WiFi应用模式为Station
  Serial.println("choose station mode");
  mySerial.print("AT+CWMODE=1\r\n");  
  delay(1000);
  printmssage();
  //连接到无线路由器 AT+CWJAP="FAST_SXM","lcx123456"\r\n
  Serial.println("connect wireless router");
  mySerial.print("AT+CWJAP=\"");
  mySerial.print(WiFiSSID);
  mySerial.print("\",\"");
  mySerial.print(WiFiPASSWORD);
  mySerial.print("\"\r\n");
  delay(10000);
  printmssage();
  //连接贝壳物联服务器
  Serial.println("connect www.bigiot.net");
  mySerial.print("AT+CIPSTART=\"TCP\",\"www.bigiot.net\",8181\r\n");
  delay(6000);
  printmssage();
  //设置模块传输模式为透传模式
  Serial.println("choose pass-through mode");
  mySerial.print("AT+CIPMODE=1\r\n");
  delay(3000);
  printmssage();
  //进入透传模式
  Serial.println("enter pass-through mode");
  mySerial.print("AT+CIPSEND\r\n");
  delay(1000);
  printmssage();
  flag=1;
  Serial.println("setting over");
  delay(2000);
  //退出透传模式，重启
  Serial.println("exit pass-through mode");
  mySerial.print("+++");
  delay(600);
  mySerial.print("\r\n");
  delay(4000);
  printmssage();
  mySerial.print("AT+RST\r\n");
  printmssage();
}
  printmssage();
}
void printmssage(){
  if (mySerial.available()){
      Serial.println(mySerial.readStringUntil('\n'));
  }
}

