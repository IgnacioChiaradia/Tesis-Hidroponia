#include <OneWire.h>
#include <DallasTemperature.h>  

#define RELE_ON 0     // Activamos el rele, ponemos valor 0 ya que los reles se activan con nivel bajo o 0 por logica inversa
#define RELE_OFF 1    // Desactivamos el rele, ponemos valor 1 ya que los reles se desactivan con nivel alto o 1 por logica inversa

// definimos pin digital a tomar 
int rele = 4;

// Parpadeo del led 13
int led_placa = 13;

// Pin donde se conecta el bus 1-Wire
const int pinDatosDQ = 2;

// Instancia a las clases OneWire y DallasTemperature
OneWire oneWireObjeto(pinDatosDQ);
DallasTemperature sensorDS18B20(&oneWireObjeto);

void setup() {
  pinMode(led_placa, OUTPUT); // declaramos el pin 13 como salida
  pinMode(rele, OUTPUT);  //configuro rele como salida

  Serial.begin(9600);

  sensorDS18B20.begin(); // seteamos el arranque del sensor
}

void loop() {
  
  Serial.println("Mandamos comandos a los sensores");
  sensorDS18B20.requestTemperatures();

  Serial.println("Temperatura sensor: ");
  Serial.println(sensorDS18B20.getTempCByIndex(0)); // obtenemos la temperatura del sensor por indice

  if(sensorDS18B20.getTempCByIndex(0) > 23)
  {
    digitalWrite(led_placa, LOW);    

    digitalWrite(rele, RELE_OFF);   //envia señal alta, por logica inversa desactivo RELE
    Serial.println("El rele se encuentra apagado");
    delay(5000);    
  }
  
  if(sensorDS18B20.getTempCByIndex(0) <= 23)
  {
    digitalWrite(led_placa, HIGH);    
    
    digitalWrite(rele, RELE_ON);  //envia señal baja, por logica inversa activo RELE
    Serial.println("El rele se encuentra encendido"); // imprimo por consola estado del RELE
    delay(5000); // seteo un tiempo de 3 segundos
  }

  delay(2000);
}
