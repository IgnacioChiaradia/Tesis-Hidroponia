//arduino UNO

#include <SoftwareSerial.h>

#include <OneWire.h>
#include <DallasTemperature.h>
#include "Temperature.h"

//Initialise Arduino to NodeMCU (5=Rx & 6=Tx)
//UNO --> COM3

const int RX_PIN = 6;
const int TX_PIN = 5;
//SoftwareSerial esp8266Serial(6,5);
SoftwareSerial esp8266Serial(RX_PIN, TX_PIN);

// Pin donde se conecta el bus 1-Wire
const int PIN_DATOS_DQ = 2;

//datos para el relevador

#define RELE_ON 0     // Activamos el rele, ponemos valor 0 ya que los reles se activan con nivel bajo o 0 por logica inversa
#define RELE_OFF 1    // Desactivamos el rele, ponemos valor 1 ya que los reles se desactivan con nivel alto o 1 por logica inversa

// definimos pin digital a tomar
#define RELE 4

// Parpadeo del led 13
const int LED_PLACA = 13;

Temperature temperatureSensor;

OneWire oneWireObjeto(PIN_DATOS_DQ);
DallasTemperature sensorDS18B20(&oneWireObjeto);

void setup() {
  pinMode(LED_PLACA, OUTPUT); // declaramos el pin 13 como salida
  pinMode(RELE, OUTPUT);  //configuro rele como salida

  digitalWrite(LED_PLACA, LOW);
  digitalWrite(RELE, RELE_OFF); // lo dejo asi para que el relevador comience apagado

  Serial.begin(9600); // Iniciamos el puerto serial del Arduino UNO
  esp8266Serial.begin(9600); //Iniciamos la comunicacion con el ESP8266

  sensorDS18B20.begin(); // seteamos el arranque del sensor

  //seteo valores por defecto
  temperatureSensor.setMinTemp("18");
  temperatureSensor.setMaxTemp("20");

  //espero 2 segundos para asegurar las conexiones
  delay(2000);
  
  Serial.println("Arduino UNO enciende");

  //envio el string encendido a la nodemcu para darle instrucciones de configuracion de ultimo rango registrado
  if (esp8266Serial.available()){
    
    esp8266Serial.print("encendido");
    esp8266Serial.print("encendido");
    esp8266Serial.print("encendido");
    Serial.print("encendido");
    Serial.print("encendido");

    delay(2000);
    esp8266Serial.print("encendido");
    esp8266Serial.print("encendido");
    esp8266Serial.print("encendido");
    Serial.print("encendido");    
  }
}

void loop() {

  Serial.println("Entra al loop del arduino UNO");
  //datos de temperatura
  Serial.println("Mandamos comandos a los sensores");
  sensorDS18B20.requestTemperatures();

  Serial.print("Temperatura sensor en arduino UNO: ");
  float temperature = sensorDS18B20.getTempCByIndex(0);
  Serial.print(temperature);
  Serial.println(" C");
  Serial.println();
  Serial.println();

  // Envío de la temperatura al ESP8266, esto lo muestro en el puerto serial del ESP8266
  //esp8266Serial.println("Temperatura 8266: " + (String)temperature + " C");
  esp8266Serial.print(temperature);

  while (esp8266Serial.available()) {
    Serial.println("entrando en el while para recibir info del ESP8266 en UNO");

    String data = esp8266Serial.readString();

    Serial.println("Informacion del ESP8266 (data)");
    Serial.println(data);

    int separatorIndexMin = data.indexOf("minTemperature");// devuelve -1 si no lo encuentra
    int separatorIndexMax = data.indexOf("maxTemperature");

    if (separatorIndexMax != -1 && separatorIndexMin != -1) {
      Serial.println("esta llegando");

      String tempMinReceived = data.substring(separatorIndexMax - 2, separatorIndexMax); // me posiciono para tomar la temperatura minima
      String tempMaxReceived = data.substring(data.length() - 2); //obtengo la temp max

      Serial.println("Esta es la info de tempMinReceived MIN:" + (String)tempMinReceived + " y esta es la info de tempMaxReceived MAX: " + (String)tempMaxReceived);

      //seteamos temperatura minima y maxima
      temperatureSensor.setMinTemp(tempMinReceived);
      temperatureSensor.setMaxTemp(tempMaxReceived);
    }
  }

  // si la lectura del sensor no da error
  if (sensorDS18B20.getTempCByIndex(0) != -127.0) {

    ///verifico que la temperatura actual NO sea demasiado alta y si lo es apago el relevador
    if (sensorDS18B20.getTempCByIndex(0) >= 28) {

      digitalWrite(RELE, RELE_OFF);
      Serial.println("Fuerzo apagado de rele por alta temperatura (28°C o mas) \n");
      
      ///verifico que la temperatura actual NO sea demasiado baja y si lo es enciendo el relevador
    } else if (sensorDS18B20.getTempCByIndex(0) <= 15) {

      digitalWrite(RELE, RELE_ON);
      Serial.println("Fuerzo encendido de rele por baja temperatura (15°C o menos) \n");
    }
    else
    {
      /////APAGAMOS O ENCENDEMOS EL RELEVADOR DEPENDIENDO DE LA TEMPERATURAS CONFIGURADAS

      if (sensorDS18B20.getTempCByIndex(0) >= temperatureSensor.getMaxTemp().toFloat()) {
        digitalWrite(LED_PLACA, LOW);

        digitalWrite(RELE, RELE_OFF);   //envia señal alta, por logica inversa desactivo RELE

        Serial.println("La temperatura actual es mayor o igual a la temp max, apagamos relevador");
        Serial.println("El rele se encuentra apagado");
      }

      if (sensorDS18B20.getTempCByIndex(0) < temperatureSensor.getMinTemp().toFloat()) {
        digitalWrite(LED_PLACA, HIGH);

        digitalWrite(RELE, RELE_ON);  //envia señal baja, por logica inversa activo RELE

        Serial.println("La temperatura actual es menor o igual a la temp min, encendemos relevador");
        Serial.println("El rele se encuentra encendido"); // imprimo por consola estado del RELE
      }
    }
  } else {
    Serial.println("Error en la lectura del sensor");
  }

  Serial.println("temperatura MINIMA " + (String)temperatureSensor.getMinTemp() + "°C y MAXIMA " + (String)temperatureSensor.getMaxTemp() + "°C" );

  digitalWrite(LED_PLACA, LOW); //apagamos el led
  delay(1000);
  digitalWrite(LED_PLACA, HIGH); //encedemos el led
  delay(2000);
}
