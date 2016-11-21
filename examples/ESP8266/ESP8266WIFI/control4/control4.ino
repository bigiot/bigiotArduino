/*
    本程序可以用来控制四路继电器
    ESP8266烧入此程序直接，使用高低电频控制光耦继电器来控制电灯
    我的继电器默认高电频关闭，所以在初始化时都初始化为高电频，play关闭开启，stop关闭关闭，输入1-4打开或关闭对应的引脚
    代码基于https://github.com/bigiot/bigiotArduino/blob/master/examples/ESP8266/kaiguan/kaiguan.ino
    上的代码进行调整，修复了部分bug，解决了断线重连问题，此代码可以直接烧入到nodemcu模块，分享代码希望对大家有帮助
*/

#include <ESP8266WiFi.h>
#include <aJSON.h>

//=============  此处必须修该============
String DEVICEID="1"; // 你的设备编号   ==
String  APIKEY = "APIKEY"; // 设备密码==
//=======================================
unsigned long lastCheckInTime = 0; //记录上次报到时间
const unsigned long postingInterval = 40000; // 每隔40秒向服务器报到一次

const char* ssid     = "ssid";//无线名称
const char* password = "password";//无线密码

const char* host = "www.bigiot.net";
const int httpPort = 8181;

int pins[4] = {D5,D6,D7,D8};
int state[4] = {HIGH,HIGH,HIGH,HIGH};
int arr_len = sizeof(pins)/sizeof(pins[0]);

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  WiFi.begin(ssid, password);
  //默认输出关闭电频
  for(int i=0;i<arr_len;i++){
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], state[i]);
  }
  
}

WiFiClient client;

void loop() {

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  // Use WiFiClient class to create TCP connections
  if (!client.connected()) {
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      delay(5000);
      return;
    }
  }

  if(millis() - lastCheckInTime > postingInterval || lastCheckInTime==0) {
    checkIn();
  }
  
  // Read all the lines of the reply from server and print them to Serial
  if (client.available()) {
    String inputString = client.readStringUntil('\n');
    inputString.trim();
    Serial.println(inputString);
    int len = inputString.length()+1;
    if(inputString.startsWith("{") && inputString.endsWith("}")){
      char jsonString[len];
      inputString.toCharArray(jsonString,len);
      aJsonObject *msg = aJson.parse(jsonString);
      processMessage(msg);
      aJson.deleteItem(msg);          
    }
  }
}

void processMessage(aJsonObject *msg){
  aJsonObject* method = aJson.getObjectItem(msg, "M");
  aJsonObject* content = aJson.getObjectItem(msg, "C");     
  aJsonObject* client_id = aJson.getObjectItem(msg, "ID");
  if (!method) {
    return;
  }
    String M = method->valuestring;
    if(M == "say"){
      String C = content->valuestring;
      String F_C_ID = client_id->valuestring;
      if(C == "play"){
        for(int i=0;i<arr_len;i++){
          state[i] = LOW;
          digitalWrite(pins[i], state[i]);
        }
        sayToClient(F_C_ID,"LED All on!");    
      }else if(C == "stop"){
        for(int i=0;i<arr_len;i++){
          state[i] = HIGH;
          digitalWrite(pins[i], state[i]);
        }
        sayToClient(F_C_ID,"LED All off!");    
      }else{
        int pin = C.toInt();
        if(pin > 0 && pin <= arr_len){
          pin--;
          state[pin] = !state[pin];
          digitalWrite(pins[pin], state[pin]);
        }
        sayToClient(F_C_ID,"LED pin:"+pin); 
      }
    }
}

void checkIn() {
    String msg = "{\"M\":\"checkin\",\"ID\":\"" + DEVICEID + "\",\"K\":\"" + APIKEY + "\"}\n";
    client.print(msg);
    lastCheckInTime = millis(); 
}

void sayToClient(String client_id, String content){
  String msg = "{\"M\":\"say\",\"ID\":\"" + client_id + "\",\"C\":\"" + content + "\"}\n";
  client.print(msg);
  lastCheckInTime = millis();
}

