#include "DHT.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

#define ssid "zafer"
#define password "12348765"

DHT dht;
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

String apiKey = "CIX5M1CTMAOIBTNS";     

boolean DEBUG=true;

void showResponse(int waitTime){
    long t=millis();
    char c;
    while (t+waitTime>millis()){
      if (Serial.available()){
        c=Serial.read();
        if (DEBUG) Serial.print(c);
      }
    }
                   
}

boolean thingSpeakWrite(float value1, float value2, float value3, float value4, float value5){
  String cmd = "AT+CIPSTART=\"TCP\",\"";                  
  cmd += "184.106.153.149";                               
  cmd += "\",80";
  Serial.println(cmd);
  if (DEBUG) Serial.println(cmd);
  if(Serial.find("Error")){
    if (DEBUG) Serial.println("AT+CIPSTART error");
    return false;
  }
  
  
  String getStr = "GET /update?api_key=";   
  getStr += apiKey;
  
  getStr +="&field1=";
  getStr += String(value1);
  getStr +="&field2=";
  getStr += String(value2);
  getStr +="&field3=";
  getStr += String(value3);
  getStr +="&field4=";
  getStr += String(value4);
  getStr +="&field5=";
  getStr += String(value5);
  // ...
  getStr += "\r\n\r\n";

  
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  Serial.println(cmd);
  if (DEBUG)  Serial.println(cmd);
  
  delay(100);
  if(Serial.find(">")){
    Serial.print(getStr);
    if (DEBUG)  Serial.print(getStr);
  }
  else{
    Serial.println("AT+CIPCLOSE");
    if (DEBUG)   Serial.println("AT+CIPCLOSE");
    return false;
  }
  return true;
}

void setup() {                
  DEBUG=true;          
  Serial.begin(115200);
  delay(100);
  Serial.println();
  dht.setup(2);
  
  if(!bmp.begin())
  {
    Serial.print("BMP180 not found ... Check your cables!");
    while(1);
  }
  
  Serial.println("AT"); 
   delay(3000); 
 analogReference(INTERNAL);
  if(Serial.find("OK")){        
     Serial.println("AT+CWMODE=1"); 
     delay(2000);
     String conn=String("AT+CWJAP=\"")+ssid+"\",\""+password+"\"";
    Serial.println(conn);
    delay(5000);
  }                                                                  
  if (DEBUG)  Serial.println("Setup completed");
}
void loop() {
  sensors_event_t event;
  bmp.getEvent(&event);
  
  delay(dht.getMinimumSamplingPeriod());
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  if (event.pressure)
  {
    float pressure= (event.pressure);
    float temperature2; 
    bmp.getTemperature(&temperature2);
    float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
    float altitude=(bmp.pressureToAltitude(seaLevelPressure,
                                            event.pressure)); 
    thingSpeakWrite(temperature,temperature2,humidity,pressure,altitude);                             
  }
  else
  {  Serial.println("Sensor error"); } 
    
  delay(20000);  
}




