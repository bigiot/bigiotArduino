/*
ESP8266 Arduino调试通讯程序
功能：Arduino通过模拟串口与ESP8266串口连接，实现通过Arduino IDE 串口监视器向ESP8266发送命令进行设置调试。
贝壳物联 http://www.bigiot.net
2015.12.14
注：在串口监视发送命令是需设置回车符为：Both NL&CR
*/
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX 模拟串口针脚
void setup()
{
  //Arduino原串口波特率设置，用于串口显示
  Serial.begin(9600);
  Serial.println("ESP8266 TEST by www.BIGIOT.net");
  //设置模拟串口波特率，与ESP8266波特率匹配，旧版可能为57600
  mySerial.begin(115200);
  //退出透传模式进行交互
  mySerial.write("+++");
  delay(1000);
  mySerial.write("\r\n");
}
void loop()
{
  //模拟串口和硬件串口数据互相调用
  if (mySerial.available()){
      Serial.write(mySerial.read());
  }
  if (Serial.available())
    mySerial.write(Serial.read());
}
