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

// variables para el manejo de la temperatura maxima y minima para el relevador
int minTemp = 26;
int maxTemp = 27;

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
  temperatureSensor.setMinTemp("26");
  temperatureSensor.setMaxTemp("27");
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
    
    //probando dato fijo para setear ambas temps al mismo tiempo ///////////////////////////////////////////////////////
    String actualTemperature = "minTemperature:20maxTemperature:27";
    data = actualTemperature;

    Serial.println("Informacion del ESP8266 (data)");
    Serial.println(data);

    int separatorIndexMax = data.indexOf("maxTemperature");
    int separatorIndexMin = data.indexOf("minTemperature");

    Serial.println("Este es el indice para la temperatura MAX:" + (String)separatorIndexMax + " y este es el valor para la temp MIN: " + (String)separatorIndexMin);

    /*TODO: repensar logica del siguiente IF ya que ahora siempre vienen ambas temperaturas a menos que no sea el string correcto 
    (algo que no tenga este formato --> minTemperature:20maxTemperature:27)*/
    
    if (separatorIndexMax != -1 || separatorIndexMin != -1) { // devuelve -1 si no lo encuentra
      Serial.println("esta llegando");

      int flagMax = 0;
      if(separatorIndexMax != -1){
        flagMax = 1;
      }
                  
      String tempIsolate = data.substring(data.length() - 2); // aislamos maxTemp, de esta manera va hasta el final del string, retrocede 2 y toma el substring a partir de ese punto
      Serial.println("Temperatura aislada de tempMax/Min VERSION2: " + (String)tempIsolate + " C");

      ///////////////

      String tempMinReceived = data.substring(separatorIndexMax - 2, separatorIndexMax); // me posiciono para tomar la temperatura minima
      String tempMaxReceived = data.substring(data.length() - 2); //obtengo la temp max

      Serial.println("Esta es la info de tempMinReceived MIN:" + (String)tempMinReceived + " y esta es la info de tempMaxReceived MAX: " + (String)tempMaxReceived);

      ///////////////

      if(flagMax){
          temperatureSensor.setMaxTemp(tempIsolate);
          Serial.println("Seteo temperatura minima "  + temperatureSensor.getMaxTemp() + " C");
      }else{
          temperatureSensor.setMinTemp(tempIsolate);
          Serial.println("Seteo temperatura minima "  + temperatureSensor.getMinTemp() + " C");
      }
    }
  }

  //APAGAMOS O ENCENDEMOS EL RELEVADOR DEPENDIENDO DE LA TEMPERATURAS SETEADAS
  
  if(sensorDS18B20.getTempCByIndex(0) >= temperatureSensor.getMaxTemp().toFloat())
  {
    digitalWrite(led_placa, LOW);    

    digitalWrite(rele, RELE_OFF);   //envia señal alta, por logica inversa desactivo RELE

    Serial.println("La temperatura actual es mayor o igual a la temp max, apagamos relevador");
    Serial.println("El rele se encuentra apagado");
  }
  
  if(sensorDS18B20.getTempCByIndex(0) < temperatureSensor.getMinTemp().toFloat()) // ver si es bueno agregar un else haciendo que se apague el rele
  {
    digitalWrite(led_placa, HIGH);    
    
    digitalWrite(rele, RELE_ON);  //envia señal baja, por logica inversa activo RELE

    Serial.println("La temperatura actual es menor o igual a la temp min, encendemos relevador");
    Serial.println("El rele se encuentra encendido"); // imprimo por consola estado del RELE
  }

  Serial.println("temperatura maxima " + (String)temperatureSensor.getMaxTemp() + "°C y minima " + (String)temperatureSensor.getMinTemp() + "°C" );

  digitalWrite(led_placa, LOW); //apagamos el led
  delay(1000);
  digitalWrite(led_placa, HIGH); //encedemos el led
  delay(2000);
}
