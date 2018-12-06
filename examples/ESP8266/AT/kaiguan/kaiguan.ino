/*
  注意 ：Arduino IDE版本请使用1.6.5-1.8.2
  ESP8266模块在接Arduino之前请确定透传稳定性，在串口查看ESP8266是否能多次收到WELCOME TO BIGIOT,
  能多次收到说明透传稳定且能自动重连。
  时间：2018.12.4
  作者：www.bigiot.net
*/

#include <aJSON.h>
//=============  此处必须修该============
String DEVICEID = "1"; // 你的设备ID   ==
String  APIKEY = "493822592"; //设备密码=
//=======================================

const int LED = 4;// LED正极连接针脚4
unsigned long lastCheckStatusTime = 0; //记录上次报到时间
const unsigned long postingInterval = 40000; // 每隔40秒向服务器报到一次
unsigned long checkoutTime = 0;//登出时间

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  delay(5000);//等一会儿ESP8266
}
void loop() {
  //每一定时间查询一次设备在线状态，同时替代心跳
  if (millis() - lastCheckStatusTime > postingInterval) {
    checkStatus();
  }
  //checkout 50ms 后 checkin
  if ( checkoutTime != 0 && millis() - checkoutTime > 50 ) {
    checkIn();
    checkoutTime = 0;
  }
  //读取串口信息
  while (Serial.available()) {
    String inputString = Serial.readStringUntil('\n');
    //检测json数据是否完整
    int jsonBeginAt = inputString.indexOf("{");
    int jsonEndAt = inputString.lastIndexOf("}");
    if (jsonBeginAt != -1 && jsonEndAt != -1) {
      //净化json数据
      inputString = inputString.substring(jsonBeginAt, jsonEndAt + 1);
      int len = inputString.length() + 1;
      char jsonString[len];
      inputString.toCharArray(jsonString, len);
      aJsonObject *msg = aJson.parse(jsonString);
      processMessage(msg);
      aJson.deleteItem(msg);
    }
  }
}
//设备登录
//{"M":"checkin","ID":"xx1","K":"xx2"}\n
void checkIn() {
  Serial.print("{\"M\":\"checkin\",\"ID\":\"");
  Serial.print(DEVICEID);
  Serial.print("\",\"K\":\"");
  Serial.print(APIKEY);
  Serial.print("\"}\r\n");
}
//处理网络接收到到指令，执行相关动作
void processMessage(aJsonObject *msg) {
  aJsonObject* method = aJson.getObjectItem(msg, "M");
  if (!method) {
    return;
  }
  String M = method->valuestring;
  if (M == "WELCOME TO BIGIOT") {
    checkOut();
    checkoutTime = millis();
    return;
  }
  if (M == "connected") {
    checkIn();
  }
  if (M == "say") {
    aJsonObject* content = aJson.getObjectItem(msg, "C");
    aJsonObject* client_id = aJson.getObjectItem(msg, "ID");
    String C = content->valuestring;
    String F_C_ID = client_id->valuestring;
    if (C == "play") {
      digitalWrite(LED, HIGH);
      say(F_C_ID, "LED on!");
    }
    if (C == "stop") {
      digitalWrite(LED, LOW);
      say(F_C_ID, "LED off!");
    }
  }
}

//发送say指令，用于设备与用户、设备与设备间通讯
//{"M":"say","ID":"xx1","C":"xx2","SIGN":"xx3"}\n
void say(String ID, String c) {
  Serial.print("{\"M\":\"say\",\"ID\":\"");
  Serial.print(ID);
  Serial.print("\",\"C\":\"");
  Serial.print(c);
  Serial.print("\"}\r\n");
}
//强制设备下线，用消除设备掉线延时
//{"M":"checkout","ID":"xx1","K":"xx2"}\n
void checkOut() {
  Serial.print("{\"M\":\"checkout\",\"ID\":\"");
  Serial.print(DEVICEID);
  Serial.print("\",\"K\":\"");
  Serial.print(APIKEY);
  Serial.print("\"}\n");
}
//查询设备在线状态
//{"M":"status"}\n
void checkStatus() {
  Serial.print("{\"M\":\"status\"}\n");
  lastCheckStatusTime = millis();
}
