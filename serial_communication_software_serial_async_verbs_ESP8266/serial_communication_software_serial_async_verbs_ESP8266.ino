
//ESP8266
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>

String actualTemperature;

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
//AsyncWebServer server(5010);

//mis archivos
#include "config.h"  
#include "ESP8266_Utils.hpp"
#include "Server.hpp"

//GPIO2 equivale a pin D4, guardamos el valor 2
const int ledPin = 2;

void setup() {  
  Serial.begin(9600); // Configura la comunicación serial a 9600 baudios e iniciamos el puerto del ESP8266
  arduinoSerial.begin(9600); // Iniciamos el puerto virtual para la comunicación con el arduino UNO
  WiFiClient wifiClient;

  pinMode(ledPin, OUTPUT);

  digitalWrite(ledPin, !state); // el led integrado funciona al revez, 0 es ON y 1 es OFF
   
  connectWifi();
  initServer();
  // Iniciar servidor
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
 
  // Lectura de datos enviados por el Arduino UNO
  while(arduinoSerial.available()) {
    Serial.println("entrando en el while ESP8266");
    
    String data = arduinoSerial.readString();
    Serial.println(data);

    actualTemperature = data;
    
    // seteo la temperatura obtenida
    temperatureSensor.set(data);

    int separatorIndex = data.indexOf('Tienequellegar');
    if (separatorIndex != -1) {
      Serial.println("esta llegando");        
    }
  }    
 
  /*digitalWrite(ledPin, LOW); //apagamos el led
  delay(1000);   
  digitalWrite(ledPin, HIGH); //encedemos el led
  delay(2000); */
}
