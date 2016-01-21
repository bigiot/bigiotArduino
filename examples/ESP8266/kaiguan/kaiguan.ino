#include <aJSON.h>
//=============  此处必须修该============
String DEVICEID="1"; // 你的设备编号   ==
String  APIKEY="493822592"; // 设备密码==
//=======================================
const int LED = 4;// LED正极连接针脚4
unsigned long lastCheckInTime = 0; //记录上次报到时间
const unsigned long postingInterval = 40000; // 每隔40秒向服务器报到一次
String inputString = "";
boolean stringComplete = false;
boolean CONNECT = true; 
char* parseJson(char *jsonString);
void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  delay(5000);
}
void loop() {
  if(millis() - lastCheckInTime > postingInterval || lastCheckInTime==0) {
    checkIn();
  }
  serialEvent();
    if (stringComplete) {
      inputString.trim();
      //Serial.println(inputString);
      if(inputString=="CLOSED"){
        Serial.println("connect closed!");
        CONNECT=false;        
      }else{
        int len = inputString.length()+1;    
        if(inputString.startsWith("{") && inputString.endsWith("}")){
          char jsonString[len];
          inputString.toCharArray(jsonString,len);
          aJsonObject *msg = aJson.parse(jsonString);
          processMessage(msg);
          aJson.deleteItem(msg);          
        }
      }      
      // clear the string:
      inputString = "";
      stringComplete = false;    
  }
}
void checkIn() {
  if (!CONNECT) {
    Serial.print("+++");
    delay(500);  
    Serial.print("\r\n"); 
    delay(1000);
    Serial.print("AT+RST\r\n"); 
    delay(6000);
    CONNECT=true;
    lastCheckInTime==0;
  }
  else{
    Serial.print("{\"M\":\"checkin\",\"ID\":\"");
    Serial.print(DEVICEID);
    Serial.print("\",\"K\":\"");
    Serial.print(APIKEY);
    Serial.print("\"}\r\n");
    lastCheckInTime = millis();   
  }
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
    if(M=="say"){
      if(C=="play"){
        digitalWrite(LED, HIGH);
        sayToClient(F_C_ID,"LED on!");    
      }
      if(C=="stop"){
        digitalWrite(LED, LOW);
        sayToClient(F_C_ID,"LED off!");    
      }
    }
}
void sayToClient(String client_id, String content){
  Serial.print("{\"M\":\"say\",\"ID\":\"");
  Serial.print(client_id);
  Serial.print("\",\"C\":\"");
  Serial.print(content);
  Serial.print("\"}\r\n");
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
