//ESP8266
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>

volatile char actualTemperature;

////////////////conexion serial//////////////
//D6 = Rx & D5 = Tx
//ESP --> COM4

//GPIO12 equivale a pin D6, guardamos el valor 12
const int rxPin = 12;

//GPIO14 equivale a pin D5, guardamos el valor 14
const int txPin = 14;

SoftwareSerial arduinoSerial(rxPin,txPin);

/////////////////AsyncWebServer//////////////////////

boolean state = false;

AsyncWebServer server(5009);

//mis archivos
#include "config.h"  
#include "ESP8266_Utils.hpp"
#include "Server.hpp"

//GPIO2 equivale a pin D4, guardamos el valor 2
const int ledPin = 2;

void setup() {  
  Serial.begin(9600); // Configura la comunicación serial a 9600 baudios
  arduinoSerial.begin(9600);

  pinMode(ledPin, OUTPUT);

  digitalWrite(ledPin, !state); // el led integrado funciona al revez
   
  connectWifi();
  initServer();
  // Iniciar servidor
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
 
  //Serial.println("Entra al loop del ESP8266");

  // Lectura de datos enviados por el Arduino UNO
  while(arduinoSerial.available()) {
    Serial.println("entrando en el while");
    
    char data = arduinoSerial.read();
    Serial.print(data);

    actualTemperature = data; // PROBAR que llega
    // ver dentro del string si esta la temperatura
  }
  
  digitalWrite(ledPin, LOW); //apagamos el led
  delay(1000);   
  digitalWrite(ledPin, HIGH); //encedemos el led
  delay(2000); 
}
