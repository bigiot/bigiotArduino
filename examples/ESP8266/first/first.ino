/*
ESP8266 Arduino UNO 调试通讯程序
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
  //发送AT测试指令
  mySerial.write("AT\r\n");
}
void loop()
{
  //模拟串口和硬件串口数据互相调用
  if (mySerial.available()){
      Serial.println(mySerial.readStringUntil('\n'));
  }
  if (Serial.available())
    mySerial.println(Serial.readStringUntil('\n'));
}
