#include <ESP8266WiFi.h>

//LED_BUILTIN
int RedLed = 16;
int GreenLed = 5;
int BlueLed = 4;
int WhiteLed = 0;
int WarmLed = 2;
int led = 2;
WiFiServer server(8080);
WiFiClient myclient;

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

void loop() {
  // put your main code here, to run repeatedly
  if(!myclient || !myclient.connected()){                      //1.if no client is connected
    myclient = server.available();                //2.Get client this 1,2 sequence will limit number of 
    if(myclient.connected()){                     //clients connected as well client will not be disconnected continiously
      Serial.println("New client connedted");
    }
  }
  else{
    //no free/disconnected spot so reject
    WiFiClient clientz = server.available();
    clientz.stop();
  
    if(myclient.available())
    {
      String request = myclient.readStringUntil('\r');
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
         int val;
         val = getInteger(request);
         analogWrite(RedLed, val);   
      }
       else if(request.substring(0,3) == "GRE")
      {
         int val;
         val = getInteger(request);
         analogWrite(GreenLed, val);   
      }
       else if(request.substring(0,3) == "BLU")
      {
         int val;
         val = getInteger(request);
         analogWrite(BlueLed, val);   
      }

       else if(request.substring(0,3) == "WHI")
      {
         int val;
         val = getInteger(request);
         analogWrite(WhiteLed, val);   
      }

       else if(request.substring(0,3) == "WRM")
      {
         int val;
         val = getInteger(request);
         analogWrite(WarmLed, val);   
      }
      myclient.write("OK\r\n");
    }
  }
}
    
    
int getInteger(String x)
{
  char dataBuffer[10];
  char intValue[5];
  x.toCharArray(dataBuffer,8);
  
  for(int i=0; i < 7; i++)
  {
       intValue[i] = dataBuffer[i+3];
  }
  intValue[4] = '\0';
   Serial.print(intValue);
  return atoi(intValue);
}
