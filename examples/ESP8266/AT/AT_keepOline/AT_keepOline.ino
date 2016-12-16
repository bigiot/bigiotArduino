#include <aJSON.h>
//=============  此处必须修该============
String DEVICEID="xxx"; // 你的设备编号   ==
String  APIKEY="xxxxxxxx"; // 设备密码==
//=======================================
unsigned long lastCheckInTime = 0; //记录上次报到时间
unsigned long lastCheckStatusTime = 0; //记录上次报到时间
const unsigned long postingInterval = 40000; // 每隔40秒向服务器报到一次
const unsigned long statusInterval = 100000; // 每隔100秒检测一次在线状态
String inputString = "";
boolean stringComplete = false;
char* parseJson(char *jsonString);
void setup() {
  Serial.begin(115200);
  delay(6000);
}
void loop() {
  if(millis() - lastCheckInTime > postingInterval || lastCheckInTime==0) {
    checkIn();
  }
  if(millis() - lastCheckStatusTime > statusInterval) {
    checkStatus();
  }
  serialEvent();
  if (stringComplete) {
    inputString.trim();
    //Serial.println(inputString);
    int len = inputString.length()+1;
    if(inputString.startsWith("{") && inputString.endsWith("}")){
      char jsonString[len];
      inputString.toCharArray(jsonString,len);
      aJsonObject *msg = aJson.parse(jsonString);
      processMessage(msg);
      aJson.deleteItem(msg);          
    }
    // clear the string:
    inputString = "";
    stringComplete = false;    
  }
}
void checkIn() {
  Serial.print("{\"M\":\"checkin\",\"ID\":\"");
  Serial.print(DEVICEID);
  Serial.print("\",\"K\":\"");
  Serial.print(APIKEY);
  Serial.print("\"}\n");
  lastCheckInTime = millis();   
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
void processMessage(aJsonObject *msg){
  aJsonObject* method = aJson.getObjectItem(msg, "M");
  aJsonObject* content = aJson.getObjectItem(msg, "C");     
  aJsonObject* client_id = aJson.getObjectItem(msg, "ID");  
  //char* st = aJson.print(msg);
  if (!method) {
    return;
  }
  //Serial.println(st); 
  //free(st);
  String M=method->valuestring;
  String C=content->valuestring;
  String F_C_ID=client_id->valuestring;
  if(M=="WELCOME TO BIGIOT"){
    checkOut();
    delay(1000);
    checkIn();
  }
  if(M=="connected"){
    checkOut();
    delay(1000);
    checkIn();
  }
  if(M=="say"){
    if(C=="offOn"){
      sayToClient(F_C_ID,"\u4f60\u5230\u5e95\u60f3\u5f00\u8fd8\u662f\u60f3\u5173\uff1f");
    }
    else if(C=="play"){
      sayToClient(F_C_ID,"\u4e00\u8d77\u73a9\u5427\u002d\u005f\u002d\u0021");
    }
    else if(C=="stop"){
      sayToClient(F_C_ID,"\u90a3\u771f\u662f\u505c\u4e0d\u4e0b\u6765\u554a\uff01");
    }else{
      sayToClient(F_C_ID,"\u4f60\u8bf4\u5565\uff1f");
    }
  }
}
void sayToClient(String client_id, String content){
  Serial.print("{\"M\":\"say\",\"ID\":\"");
  Serial.print(client_id);
  Serial.print("\",\"C\":\"");
  Serial.print(content);
  Serial.print("\"}\n");
  lastCheckInTime = millis();
}
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
