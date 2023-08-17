#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>

#define LED_BUILTIN 2

boolean state = false;

AsyncWebServer server(5009);

//mis archivos
#include "config.h"  
#include "ESP8266_Utils.hpp"
#include "Server.hpp"
 
void setup(void) 
{
   Serial.begin(115200);

   pinMode(LED_BUILTIN, OUTPUT);
   digitalWrite(LED_BUILTIN, !state); // el led integrado funciona al revez
   
   connectWifi();
   initServer();
   // Iniciar servidor
   server.begin();
   Serial.println("HTTP server started");
}
 
void loop()
{
  //como es asincrono ya no necesitamos este handleClient
  //server.handleClient();
}
