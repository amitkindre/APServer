#include <ESP8266WiFi.h>

//LED_BUILTIN
int RedLed = 16;    //D0
int GreenLed = 5;   //D1
int BlueLed = 4;    //D2
int WhiteLed = 14;  //D5
int WarmLed = 12;   //D6
int led = 2;
WiFiServer server(8080);
WiFiClient myclient;
IPAddress clientIP;
char* ssid = "SSEServer";





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

//  myIP =  WiFi.localIP(); //Get IP address
//  Serial.print("Local IP:");
//  Serial.println(myIP);

}

 String request;
 WiFiClient clientz ;
 IPAddress clientzIP; 
 int val;

void loop() {
  
  // put your main code here, to run repeatedly
  if(!myclient || !myclient.connected()){                      //1.if no client is connected
      myclient = server.available();                //2.Get client this 1,2 sequence will limit number of 
      if(myclient.connected()){                     //clients connected as well client will not be disconnected continiously
        digitalWrite(led, HIGH);  
        clientIP = myclient.remoteIP();
        Serial.print("New client connedted : ");
        Serial.println(clientIP);
      }
      else{
          digitalWrite(led, LOW);  
      }
  }
  else{
    //no free/disconnected spot so reject
    clientz = server.available();
    if(clientz){ 
      Serial.print("Client z");
      clientzIP = clientz.remoteIP();
      if(clientIP == clientzIP)             //old client is reconnected
        myclient = clientz;
      else
        clientz.stop();
    }
  
    if(myclient.available())
    {
      request = myclient.readStringUntil('\r');
      myclient.flush();
      Serial.println(request);
      if(request[5] == 'O')
      {
         analogWrite(led, 0);
      }
      else if(request[5] == 'C')
      {
         analogWrite(led, 1024);
      }
      else if(request.substring(0,3) == "RED")
      {
         val = getInteger(request);
         analogWrite(RedLed, val);   
      }
       else if(request.substring(0,3) == "GRE")
      {
         val = getInteger(request);
         analogWrite(GreenLed, val);   
      }
       else if(request.substring(0,3) == "BLU")
      {
         val = getInteger(request);
         analogWrite(BlueLed, val);   
      }

       else if(request.substring(0,3) == "WHI")
      {
         val = getInteger(request);
         analogWrite(WhiteLed, val);   
      }

       else if(request.substring(0,3) == "WRM")
      {
         val = getInteger(request);
         analogWrite(WarmLed, val);   
      }
      myclient.write("OK\r\n");
    }
  }
}

char dataBuffer[10];
char intValue[5];    
int getInteger(String x)
{
  
  x.toCharArray(dataBuffer,8);
  
  for(int i=0; i < 7; i++)
  {
       intValue[i] = dataBuffer[i+3];
  }
  intValue[4] = '\0';
   Serial.print(intValue);
  return atoi(intValue);
}
