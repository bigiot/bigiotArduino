#include <SPI.h>
#include <Ethernet.h>
#include <aJSON.h>
//=============  此处必须修改============
const String  APIKEY="493822592"; // 此处替换为你自己的API KEY
const String DEVICEID="1"; // 此处替换为你的设备编号
const String INPUTID="1";
//=======================================
const int LM35 = 0;//LM35 pin
byte mac[] = {0x00, 0x1D, 0x72, 0x82, 0x35, 0x9D};
EthernetClient client ;
IPAddress ip(192, 168, 0, 177);//local IP
//IPAddress server(121,42,180,30);
char server[] = "www.bigiot.net";  
int port= 8181 ;

aJsonStream serial_stream(&client);
unsigned long lastCheckInTime = 0; 
unsigned long lastUpdateTime = 0; 
const unsigned long postingInterval = 40000; // delay between 2 datapoints, 40s
const unsigned long updateInterval = 5000; // delay between 2 datapoints, 5s
boolean isCheckIn = false;

void setup() {
  Serial.begin(9600);
  if (Ethernet.begin(mac) == 0) {// start the Ethernet connection with DHCP:
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, ip);   
  }
  else {
    Serial.println("Ethernet configuration OK");
  } 
  delay(10000);
  Serial.println("connecting...");
  if(client.connect(server, port)){
    Serial.println("connected");
  }else{
    Serial.println("connection failed");
  }
}

void loop() {
  if(millis() - lastCheckInTime > postingInterval) {
    Serial.println("checking in"); 
    checkIn();
  }
  if((millis() - lastUpdateTime > updateInterval) && isCheckIn) {
    int val;//定义变量
    int dat;//定义变量
    dat=analogRead( LM35 );// 读取传感器的模拟值并赋值给dat
    //val=(125*dat)>>8;//温度计算公式
    val = dat * (4.76 / 1023.0*100);  
    update1(DEVICEID,INPUTID,val);
  }
 if (serial_stream.available()) {
    /* First, skip any accidental whitespace like newlines. */
    serial_stream.skip();
  }
  if (serial_stream.available()) {
    /* Something real on input, let's take a look. */
    aJsonObject *msg = aJson.parse(&serial_stream);
    processMessage(msg);
    aJson.deleteItem(msg);
  } 
}

void checkIn() {
  if (!client.connected()) {
    Serial.println("connection failed"); 
    client.stop();
    isCheckIn=false;
    client.connect(server, port);
    Serial.println("connecting..."); 
    delay(10000);
  }
  else{
    Serial.println("connection success"); 
    client.print("{\"M\":\"checkin\",\"ID\":\"");
    client.print(DEVICEID);
    client.print("\",\"K\":\"");
    client.print(APIKEY);
    client.println("\"}");
    lastCheckInTime = millis();
    Serial.println("checking...");    
  }
}

void processMessage(aJsonObject *msg){
  aJsonObject* method = aJson.getObjectItem(msg, "M");
  String M=method->valuestring;
  char* st = aJson.print(msg);        
  if (st != NULL) {
    Serial.println(st); 
    free(st);
    if(M=="checkinok" ){      
      isCheckIn=true;
      Serial.println("check in OK!");   
    }
  } 
}

void update1(String did, String inputid, int value){
  client.print("{\"M\":\"update\",\"ID\":\"");
  client.print(did);
  client.print("\",\"V\":{\"");
  client.print(inputid);
  client.print("\":\"");
  client.print(value);
  client.println("\"}}");
  lastCheckInTime = millis();
  lastUpdateTime= millis();
  Serial.print("update:");   
  Serial.print(inputid);   
  Serial.print("->");   
  Serial.println(value);   
}
//同时上传两个接口数据调用此函数
void update2(String did, String inputid1, int value1, String inputid2, int value2){
  client.print("{\"M\":\"update\",\"ID\":\"");
  client.print(did);
  client.print("\",\"V\":{\"");
  client.print(inputid1);
  client.print("\":\"");
  client.print(value1);
  client.print("\",\"");
  client.print(inputid2);
  client.print("\":\"");
  client.print(value2);
  client.println("\"}}");
  lastCheckInTime = millis();
  lastUpdateTime= millis();
  Serial.print("update:");   
  Serial.print(inputid);   
  Serial.print("->");   
  Serial.println(value);   
}
