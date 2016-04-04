#include <SPI.h>
#include <Ethernet.h>
#include <aJSON.h>
//=============  此处必须修改============
String  APIKEY="9cfe676a9"; // 此处替换为你自己的API KEY
String DEVICEID="4"; // 此处替换为你的设备编号
//=======================================
const int LED = 3;//
byte mac[] = {0x00, 0x1D, 0x72, 0x82, 0x35, 0x9D};
EthernetClient client ;
IPAddress ip(192, 168, 0, 177);//local IP
//IPAddress server(121,42,180,30);
char server[] = "www.bigiot.net";  
int port= 8181 ;

aJsonStream serial_stream(&client);
unsigned long lastCheckInTime = 0; 
const unsigned long postingInterval = 40000; // delay between 2 datapoints, 30s

void setup() {
  pinMode(LED, OUTPUT);
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
    Serial.println("connection fiald");   
    client.stop();
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
    Serial.println("check in OK!");    
  }
}
void processMessage(aJsonObject *msg){
  aJsonObject* method = aJson.getObjectItem(msg, "M");
  aJsonObject* content = aJson.getObjectItem(msg, "C");     
  aJsonObject* client_id = aJson.getObjectItem(msg, "ID");  
  char* st = aJson.print(msg);        
  if (st != NULL) {
    Serial.println(st); 
    free(st);
    String M=method->valuestring;
    String C=content->valuestring;
    String F_C_ID=client_id->valuestring;
    if(M=="say" ){
      if(C=="play"){
        digitalWrite(LED, HIGH);
        sayToClient(F_C_ID,"Buzzing!");    
      }else if(C=="stop"){
        digitalWrite(LED, LOW);
        sayToClient(F_C_ID,"Stop Buzzing!");    
      }else{
        sayToClient(F_C_ID,"unknown command!"); 
      }
    }
  } 
  else {
    Serial.println( "wrong");
  }
}
void sayToClient(String client_id, String content){
  client.print("{\"M\":\"say\",\"ID\":\"");
  client.print(client_id);
  client.print("\",\"C\":\"");
  client.print(content);
  client.println("\"}");
  lastCheckInTime = millis();
  Serial.print("say to ");    
  Serial.print(client_id);    
  Serial.print(":");  
  Serial.println(content);    
}
