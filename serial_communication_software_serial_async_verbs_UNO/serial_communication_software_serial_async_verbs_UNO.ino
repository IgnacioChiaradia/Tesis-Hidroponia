//arduino UNO

#include <SoftwareSerial.h>

#include <OneWire.h>
#include <DallasTemperature.h>
#include "Temperature.h"

//Initialise Arduino to NodeMCU (5=Rx & 6=Tx)
//UNO --> COM3

const int rxPin = 6;
const int txPin = 5;
//SoftwareSerial esp8266Serial(6,5);
SoftwareSerial esp8266Serial(rxPin, txPin);

// Pin donde se conecta el bus 1-Wire
const int pinDatosDQ = 2;

//datos para el relevador

#define RELE_ON 0     // Activamos el rele, ponemos valor 0 ya que los reles se activan con nivel bajo o 0 por logica inversa
#define RELE_OFF 1    // Desactivamos el rele, ponemos valor 1 ya que los reles se desactivan con nivel alto o 1 por logica inversa

// definimos pin digital a tomar 
int rele = 4;

// Parpadeo del led 13
int led_placa = 13;

Temperature temperatureSensor; 

OneWire oneWireObjeto(pinDatosDQ);
DallasTemperature sensorDS18B20(&oneWireObjeto);

void setup() {
  pinMode(led_placa, OUTPUT); // declaramos el pin 13 como salida
  pinMode(rele, OUTPUT);  //configuro rele como salida

  digitalWrite(led_placa, LOW);
  digitalWrite(rele, RELE_OFF); // lo dejo asi para que el relevador comience apagado     

  Serial.begin(9600); // Iniciamos el puerto serial del Arduino UNO
  esp8266Serial.begin(9600);

  sensorDS18B20.begin(); // seteamos el arranque del sensor

  //seteo valores iniciales
  temperatureSensor.setMinTemp("17");
  temperatureSensor.setMaxTemp("19");
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

  /////APAGAMOS O ENCENDEMOS EL RELEVADOR DEPENDIENDO DE LA TEMPERATURAS SETEADAS
  
  if(sensorDS18B20.getTempCByIndex(0) >= temperatureSensor.getMaxTemp().toFloat())
  {
    digitalWrite(led_placa, LOW);    

    digitalWrite(rele, RELE_OFF);   //envia señal alta, por logica inversa desactivo RELE

    Serial.println("La temperatura actual es mayor o igual a la temp max, apagamos relevador");
    Serial.println("El rele se encuentra apagado");
  }
  
  if(sensorDS18B20.getTempCByIndex(0) < temperatureSensor.getMinTemp().toFloat()) 
  {
    digitalWrite(led_placa, HIGH);    
    
    digitalWrite(rele, RELE_ON);  //envia señal baja, por logica inversa activo RELE

    Serial.println("La temperatura actual es menor o igual a la temp min, encendemos relevador");
    Serial.println("El rele se encuentra encendido"); // imprimo por consola estado del RELE
  }
    
  Serial.println("temperatura MINIMA " + (String)temperatureSensor.getMinTemp() + "°C y MAXIMA " + (String)temperatureSensor.getMaxTemp() + "°C" );

  digitalWrite(led_placa, LOW); //apagamos el led
  delay(1000);
  digitalWrite(led_placa, HIGH); //encedemos el led
  delay(2000);
}
