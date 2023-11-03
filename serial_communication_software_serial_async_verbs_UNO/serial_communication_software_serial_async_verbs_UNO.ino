//arduino UNO

#include <SoftwareSerial.h>

#include <OneWire.h>
#include <DallasTemperature.h>

//Initialise Arduino to NodeMCU (5=Rx & 6=Tx)
//UNO --> COM3

const int rxPin = 6;
const int txPin = 5;
//SoftwareSerial esp8266Serial(6,5);
SoftwareSerial esp8266Serial(rxPin, txPin);

// Pin donde se conecta el bus 1-Wire
const int pinDatosDQ = 2;

OneWire oneWireObjeto(pinDatosDQ);
DallasTemperature sensorDS18B20(&oneWireObjeto);

void setup() {
  pinMode(13, OUTPUT); // declaramos el pin 13 como salida

  Serial.begin(9600); // Iniciamos el puerto serial del Arduino UNO
  esp8266Serial.begin(9600);

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
  //esp8266Serial.println("Temperatura 8266: " + (String)temperature + " C");
  esp8266Serial.print(temperature);
  //esp8266Serial.println("Tienequellegar");

  while (esp8266Serial.available()) {
    Serial.println("entrando en el while para recibir info del ESP8266 en UNO");                                                                                                                                                                                                                                                                                                                                                                    

    String data = esp8266Serial.readString();

    String actualTemperature = data; // PROBAR que llega
    // ver dentro del string si esta la temperatura

    Serial.println("Informacion del ESP8266");
    Serial.println(actualTemperature);

    int separatorIndexMax = data.indexOf('maxTemperature');
    int separatorIndexMin = data.indexOf('minTemperature');
    if (separatorIndexMax != -1 || separatorIndexMin != -1) { // devuelve -1 si no lo encuentra
      Serial.println("esta llegando");
            
      String tempMaxIsolate = data.substring(data.length() - 2); // probando aislar maxTemp, de esta manera va hasta el final del string, retrocede 2 y toma el substring a partir de ese punto
      Serial.println("Temperatura aislada de tempMax/Min VERSION2: " + (String)tempMaxIsolate + " C");      
    }
  }

  digitalWrite(13, LOW); //apagamos el led
  delay(1000);
  digitalWrite(13, HIGH); //encedemos el led
  delay(2000);
}
