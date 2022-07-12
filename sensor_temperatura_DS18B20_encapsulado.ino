#include <OneWire.h>
#include <DallasTemperature.h>  
 
// Parpadeo del led 13

int led_placa = 13;

// Pin donde se conecta el bus 1-Wire
const int pinDatosDQ = 9;

// Instancia a las clases OneWire y DallasTemperature
OneWire oneWireObjeto(pinDatosDQ);
DallasTemperature sensorDS18B20(&oneWireObjeto);

void setup() {
  pinMode(led_placa, OUTPUT); // declaramos el pin 13 como salida

  Serial.begin(9600);

  sensorDS18B20.begin(); // seteamos el arranque del sensor
}

void loop() {
  
  Serial.println("Mandamos comandos a los sensores");
  sensorDS18B20.requestTemperatures();

  Serial.println("Temperatura sensor: ");
  Serial.println(sensorDS18B20.getTempCByIndex(0)); // obtenemos la temperatura del sensor por indice

  if(sensorDS18B20.getTempCByIndex(0) >= 21)
  {
    digitalWrite(led_placa, HIGH);    
  }
  else
  {
    digitalWrite(led_placa, LOW);    
  }

  delay(2000);
}
