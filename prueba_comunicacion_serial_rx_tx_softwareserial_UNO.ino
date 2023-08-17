//arduino UNO

#include <SoftwareSerial.h>

#include <OneWire.h>
#include <DallasTemperature.h>  

//Initialise Arduino to NodeMCU (5=Rx & 6=Tx)
//UNO --> COM3

const int rxPin = 6;
const int txPin = 5;
//SoftwareSerial esp8266Serial(D6, D5);
SoftwareSerial esp8266Serial(rxPin,txPin);

// Pin donde se conecta el bus 1-Wire
const int pinDatosDQ = 2;

OneWire oneWireObjeto(pinDatosDQ);
DallasTemperature sensorDS18B20(&oneWireObjeto);

void setup() {
  pinMode(13, OUTPUT); // declaramos el pin 13 como salida
  
  Serial.begin(9600); // Iniciamos el puerto serial del Arduino UNO
  esp8266Serial.begin(9600); // Iniciamos el puerto virtual para la comunicación con el ESP8266

  sensorDS18B20.begin(); // seteamos el arranque del sensor
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
  esp8266Serial.print("Temperatura 8266: ");
  esp8266Serial.print(temperature);
  esp8266Serial.println(" C");
  
  digitalWrite(13, LOW); //apagamos el led
  delay(1000);   
  digitalWrite(13, HIGH); //encedemos el led
  delay(2000);    
}
