
//ESP8266
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>

//client http
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

//json
#include <ArduinoJson.h>

String actualTemperature;

////////////////conexion serial//////////////
//D6 = Rx & D5 = Tx
//ESP --> COM4

//GPIO12 equivale a pin D6, guardamos el valor 12
const int RX_PIN = 12;

//GPIO14 equivale a pin D5, guardamos el valor 14
const int TX_PIN = 14;

SoftwareSerial arduinoSerial(RX_PIN, TX_PIN);

/////////////////AsyncWebServer//////////////////////

boolean state = false;

AsyncWebServer server(5009);

//mis archivos
#include "config.h"
#include "Server.hpp"
#include "ESP8266_Utils.hpp"

//GPIO2 equivale a pin D4, guardamos el valor 2
const int LED_PIN = 2;

void setup() {
  Serial.begin(9600); // Configura la comunicación serial a 9600 baudios e iniciamos el puerto del ESP8266
  arduinoSerial.begin(9600); // Iniciamos el puerto virtual para la comunicación con el arduino UNO

  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, !state); // el led integrado funciona al revez, 0 es ON y 1 es OFF

  connectWifi();
  initServer();
  // Iniciar servidor
  server.begin();
  Serial.println("HTTP server started");

  //espero 2 segundos para asegurar las conexiones
  delay(2000);

  //Realiso seteo inicial del rango por posible conexion y desconeccion del arduino UNO
  setInitialRange();
}

void loop() {

  // Lectura de datos enviados por el Arduino UNO
  while (arduinoSerial.available()) {
    Serial.println("entrando en el while ESP8266");

    String data = arduinoSerial.readString();
    Serial.println(data);

    actualTemperature = data;

    int separatorIndex = data.indexOf("encendido");
    Serial.println("separatorIndex: " + (String)separatorIndex);
        
    if (separatorIndex != -1) {
      Serial.println("el arduino UNO inicio !, enviamos peticion para configurar rango");
      setInitialRange();
    } else {
      Serial.println("no recibe el ENCENDIDO");

      // seteo la temperatura obtenida
      temperatureSensor.set(data);
    }
  }
}
