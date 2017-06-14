#include <ESP8266WiFi.h>

//LED_BUILTIN
int led = 16;

WiFiServer server(8080);
WiFiClient myclient;

char* ssid = "ESPServer";


void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  Serial.println("");
  pinMode(led, OUTPUT);

  WiFi.mode(WIFI_AP); //Only Access point
  WiFi.softAP(ssid);  //Start HOTspot removing password will disable security

  IPAddress myIP = WiFi.softAPIP(); //Get IP address
  Serial.print("HotSpt IP:");
  Serial.println(myIP);

  
  server.begin(); 
  Serial.println("Server Started");
  myIP =  WiFi.localIP(); //Get IP address
  Serial.print("Local IP:");
  Serial.println(myIP);
 

}

void loop() {
  // put your main code here, to run repeatedly:

  
  myclient = server.available();

  if(!myclient){
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while(!myclient.available()){
    delay(1);
  }

  //read first line
  String request = myclient.readStringUntil('\r');
  Serial.println(request);
  if(request[5] == 'O')
  {
     digitalWrite(led, LOW);
  }
  if(request[5] == 'C')
  {
     digitalWrite(led, HIGH);
  }
  myclient.flush();
  //myclient.print("OK");
}
