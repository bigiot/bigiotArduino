#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); 
unsigned long lastCheckInTime = 0; 
unsigned long lastUpdateTime = 0; 
const unsigned long postingInterval = 30000; // delay between 2 datapoints, 30s
const unsigned long updateInterval = 5000; // delay between 2 datapoints, 30s
void setup() {
  Serial.begin(9600);
  mySerial.begin(115200);
  delay(6000);
  mySerial.print("+++");
  delay(600);
  mySerial.print("\r\n");  
  delay(1000);
  mySerial.print("AT+CWMODE=1\r\n");  
  delay(1000);
  mySerial.print("AT+CWJAP=\"FAST_SXM\",\"lcx123456\"\r\n");
  delay(2000);
  mySerial.print("AT+CIPSTART=\"TCP\",\"192.168.1.100\",8181\r\n");
  delay(2000);
  mySerial.print("AT+CIPMODE=1\r\n");
  delay(1000);
  mySerial.print("AT+CIPSEND\r\n");
  delay(500);
  mySerial.print("+++");
  delay(600);
  mySerial.print("\r\n");  
  delay(1000);
  mySerial.print("AT+RST\r\n");  
  delay(6000);
}

void loop() {
  if(millis() - lastCheckInTime > postingInterval || lastCheckInTime==0) {
  String checkin="{\"M\":\"checkin\",\"ID\":\"1\",\"K\":\"770739091\"}";
  mySerial.print(checkin);  
  mySerial.print("\r\n");  
  lastCheckInTime = millis();
  }
  if (mySerial.available()){
      Serial.println(mySerial.readStringUntil('\n'));
  }
}
