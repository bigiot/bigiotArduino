/*
  注意 ：Arduino IDE版本请使用1.6.5-1.8.2
  ESP8266模块在接Arduino之前请确定透传稳定性，在串口查看ESP8266是否能多次收到WELCOME TO BIGIOT,
  能多次收到说明透传稳定且能自动重连。
  时间：2018.12.4
  作者：www.bigiot.net
*/
#include <aJSON.h>
//=============  此处必须修改============
String DEVICEID = "110"; // 你的设备ID=======
String APIKEY = "c88036f9c"; // 设备密码==
String INPUTID = "4"; //接口ID==============
//=======================================
const int LM35 = 0;// LM35 Vout 接 A0
unsigned long lastCheckStatusTime = 0; //记录上次报到时间
unsigned long lastUpdateTime = 0;//记录上次上传数据时间
const unsigned long postingInterval = 40000; // 每隔40秒向服务器报到一次
const unsigned long updateInterval = 5000; // 数据上传间隔时间5秒
unsigned long checkoutTime = 0;//登出时间
void setup() {
  Serial.begin(115200);
  delay(3000);
}
void loop() {
  if (millis() - lastCheckStatusTime > postingInterval) {
    checkStatus();
  }
  //checkout 50ms 后 checkin
  if ( checkoutTime != 0 && millis() - checkoutTime > 50 ) {
    checkIn();
    checkoutTime = 0;
  }
  if (millis() - lastUpdateTime > updateInterval) {
    float val;//定义变量
    int dat;//定义变量
    dat = analogRead( LM35 ); // 读取传感器的模拟值并赋值给dat
    //val=(125*dat)>>8;//温度计算公式
    val = dat * (4.76 / 1023.0 * 100);
    update1(DEVICEID, INPUTID, val);
    lastUpdateTime = millis();
  }
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
void checkIn() {
  Serial.print("{\"M\":\"checkin\",\"ID\":\"");
  Serial.print(DEVICEID);
  Serial.print("\",\"K\":\"");
  Serial.print(APIKEY);
  Serial.print("\"}\n");
}
void checkOut() {
  Serial.print("{\"M\":\"checkout\",\"ID\":\"");
  Serial.print(DEVICEID);
  Serial.print("\",\"K\":\"");
  Serial.print(APIKEY);
  Serial.print("\"}\n");
}
void checkStatus() {
  Serial.print("{\"M\":\"status\"}\n");
  lastCheckStatusTime = millis();
}
//处理来自ESP8266透传的数据
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
}
//上传一个接口数据
//{"M":"update","ID":"2","V":{"2":"120"}}\n
void update1(String did, String inputid, float value) {
  Serial.print("{\"M\":\"update\",\"ID\":\"");
  Serial.print(did);
  Serial.print("\",\"V\":{\"");
  Serial.print(inputid);
  Serial.print("\":\"");
  Serial.print(value);
  Serial.println("\"}}");
}
//同时上传两个接口数据调用此函数
//{"M":"update","ID":"112","V":{"6":"1","36":"116"}}\n
void update2(String did, String inputid1, float value1, String inputid2, float value2) {
  Serial.print("{\"M\":\"update\",\"ID\":\"");
  Serial.print(did);
  Serial.print("\",\"V\":{\"");
  Serial.print(inputid1);
  Serial.print("\":\"");
  Serial.print(value1);
  Serial.print("\",\"");
  Serial.print(inputid2);
  Serial.print("\":\"");
  Serial.print(value2);
  Serial.println("\"}}");
}
//上传更多数据，可以参考上面的写法，和通讯协议，自己添加。
//贝壳物联通讯协议：https://www.bigiot.net/help/1.html
