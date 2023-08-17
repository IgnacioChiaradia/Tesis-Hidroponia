//ESP8266

#include <SoftwareSerial.h>

//D6 = Rx & D5 = Tx
//ESP --> COM4

//GPIO12 equivale a pin D6, guardamos el valor 12
const int rxPin = 12;

//GPIO14 equivale a pin D5, guardamos el valor 14
const int txPin = 14;

SoftwareSerial arduinoSerial(rxPin,txPin);

//const char ledPin[] = "D4";
//GPIO2 equivale a pin D4, guardamos el valor 2
const int ledPin = 2;

void setup() {
  pinMode(ledPin, OUTPUT);
  
  Serial.begin(9600); // Configura la comunicación serial a 9600 baudios
  arduinoSerial.begin(9600);
}

void loop() {
 
  Serial.println("Entra al loop del ESP8266");

  // Lectura de datos enviados por el Arduino UNO
  while(arduinoSerial.available()) {
    //Serial.println("entrando en el while");
    char data = arduinoSerial.read();
    //Serial.write(data); // Muestra los datos en el monitor serial del ESP8266
    //Serial.write(Serial.read());
    Serial.print(data);
  }
  
  digitalWrite(ledPin, LOW); //apagamos el led
  delay(1000);   
  digitalWrite(ledPin, HIGH); //encedemos el led
  delay(2000); 
}
