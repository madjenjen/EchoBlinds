/*
  Control servo motors through Amazon Echo
   Based on the source code from: http://www.seeedstudio.com/wiki/WiFi_Serial_Transceiver_Module
*/

#define cs   10  // Pins for the display
#define dc   9
#define rst  8 

#include "config.h"

const char ssid[] = SSID;
const char pass[] = PASS;
const char dst_ip[] = DST_IP;

void setup()
{
  Serial.begin(9600);
  Serial2.begin(115200);
  Serial2.setTimeout(5000);
  Serial.println("Init");
  delay(1000);
  if(Serial2.find("ready")) {
    Serial.println("WiFi - Module is ready");
  }else{
    Serial.println("Module dosn't respond.");
    //while(1);
  }
  delay(1000);
  // try to connect to wifi
  boolean connected=false;
  for(int i=0;i<5;i++){
    if(connectWiFi()){
      connected = true;
      Serial.println("Connected to WiFi...");
      break;
    }
  }
  if (!connected){
    Serial.println("Coudn't connect to WiFi.");
    while(1);
  }
  delay(5000);
  Serial2.println("AT+CIPMUX=0"); // set to single connection mode
}
void loop()
{
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += DST_IP;
  cmd += "\",80";
  Serial2.println(cmd);
  Serial.println(cmd);
  if(Serial2.find("Error")) return;
  cmd = "GET /test/";
  cmd += " HTTP/1.0\r\n\r\n";
  Serial2.print("AT+CIPSEND=");
  Serial2.println(cmd.length());
  Serial.println(cmd);
  if(Serial2.find(">")){
    Serial.print(">");
  }else{
    Serial2.println("AT+CIPCLOSE");
    Serial.println("connection timeout");
    delay(1000);
    return;
  }
  Serial2.print(cmd);
  unsigned int i = 0; //timeout counter
  int n = 1; // char counter
  char json[100]="{";
  while (!Serial2.find("{")){}
  while (i<6000) {
    if(Serial2.available()) {
      char c = Serial2.read();
      Serial.write(c);
      json[n]=c;
      n++;
      i=0;
    }
    i++;
  }
  Serial.println(json);
  /*JsonObject root = parser.parse(json);
  double temp = root["temp"];
  double pressure = root["pressure"];
  double humidity = root["humidity"];
  temp -= 273.15; // from kelvin to degree celsius
  dbgSerial.println(temp);
  dbgSerial.println(pressure);
  dbgSerial.println(humidity);
  dbgSerial.println("====");*/
  delay(60000);
}
     
boolean connectWiFi()
{
  Serial2.println("AT+CWMODE=1");
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  Serial2.println(cmd);
  delay(2000);
  if(Serial2.find("OK")){
    Serial.println("OK, Connected to WiFi.");
    return true;
  }else{
    Serial.println("Can not connect to the WiFi.");
    return false;
  }
}