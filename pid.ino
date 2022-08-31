#include <PID_v1.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define pinData 2
#define SSR 6  //rele de estado solido.
#define led 13
#define tiempoCiclo 1000

double Setpoint, Input, Output;
double Kp=10, Ki=3, Kd=400;
float temperatura = 0;

unsigned long respuestaUltimaTemperatura = 0;
unsigned long lastPIDCalculation = 0;
float prevtemperatua = -9999.0;

PID myPID (&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
OneWire ourWire(pinData);
DallasTemperature sensors(&ourWire);

void setup() {
  Setpoint = 40.0;
  myPID.SetOutputLimits(0, tiempoCiclo);
  myPID.SetSampleTime(tiempoCiclo);
  myPID.SetMode(AUTOMATIC);
  Serial.begin(115200);//grafica datos para ver por pantalla
  //pinMode(SSR, OUTPUT);
  //digitalWrite(SSR, LOW);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  sensors.begin();
}

void loop() {
  
  if(millis() - respuestaUltimaTemperatura >= tiempoCiclo){
    temperatura = sensors.getTempCByIndex(0); //busqueda por hexadecimal
    Input = (double)temperatura;
     myPID.Compute();
    lastPIDCalculation = millis();
    sensors.requestTemperatures();
    respuestaUltimaTemperatura = millis();
  }
  control();
}

void control(){
  if((millis() <= (lastPIDCalculation + Output)) || (Output == tiempoCiclo)){
    //Power on:
    //digitalWrite(SSR, HIGH);
    digitalWrite(led, HIGH);
    
  }
  else{
   //digitalWrite(SSR, LOW);
   digitalWrite(led, LOW);
  }  
  
}
