#include "Temperature.h"
#include <ArduinoJson.h>

// esta variable se usa para guardar desde el main y la devolvemos en la response desde aca
Temperature temperatureSensor;

void handleFormLed(AsyncWebServerRequest *request)
{
  String ledStatus = request->arg("status");

  Serial.print("Status:\t");
  Serial.println(ledStatus);

  digitalWrite(LED_BUILTIN, ledStatus == "ON" ? LOW : HIGH);

  AsyncWebServerResponse *response = request->beginResponse(200, "application/json", "Hello World!");
  response->addHeader("Server", "ESP Async Web Server");
  request->send(response);

  request->redirect("/");
}

void handleProof(AsyncWebServerRequest *request)
{
  Serial.print("Entra a handleProof\t");

  request->send(200, "application/json", "Funciona el GET");
}

void getTemperature(AsyncWebServerRequest *request) {

  Serial.print("Recuperando temperatura actual:\t");
  Serial.println(temperatureSensor.get());

  //Crear un objeto JSON, usamos StaticJsonDocument ya que el documento es pequeño
  /*const int capacity = JSON_OBJECT_SIZE(3);
  StaticJsonDocument<capacity> jsonDoc;

  // Agregar datos al objeto JSON
  jsonDoc["nombre"] = "TempActual";
  jsonDoc["valor"] = temperatureSensor.get();

  // Declare un buffer para contener el resultado
  char result[128];
  serializeJsonPretty(jsonDoc, result);

  AsyncWebServerResponse *response = request->beginResponse(200, "application/json", result);*/

  // reviso que el sensor este devolviendo una temperatura valida ya que -127.0 significa que hubo un fallo en la lectura
  if(temperatureSensor.get() != "-127.0"){ 
    //por ahora solo usamos el dato de la temperatura y lo enviamos en formato json
    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", temperatureSensor.get());
    response->addHeader("Server message", "getTemperature");
    request->send(response);    
  } else {
    request->send(404, "application/json", "Not found getTemperature");
  }

  //jsonBuffer.clear();

  //request->redirect("/");
}

//SET TEMPERATURA MAXIMA Y MINIMA
void setRangeTemperature(AsyncWebServerRequest *request) {

  Serial.print("Mostrando tempMin y Max pedidas: ");

  if (request->hasParam("minTemperature") && request->hasParam("maxTemperature")) {
    AsyncWebParameter* tempMin = request->getParam("minTemperature");
    AsyncWebParameter* tempMax = request->getParam("maxTemperature");

    temperatureSensor.setMinTemp(tempMin->value().c_str());
    String minTemperature = tempMin->value().c_str();

    temperatureSensor.setMaxTemp(tempMax->value().c_str());
    String maxTemperature = tempMax->value().c_str();

    Serial.println(minTemperature + " y " + maxTemperature);

    AsyncWebServerResponse *response = request->beginResponse(200);
    response->addHeader("Server message", "POST_setRangeTemperature_work");
    request->send(response);

    //ENVIO DE MAXTEMP y MINTEMP a UNO, usamos minTemperature y maxTemperature como keys
    arduinoSerial.print("minTemperature:" + temperatureSensor.getMinTemp() + "maxTemperature:" + temperatureSensor.getMaxTemp());
    
  } else {
    request->send(404, "application/json", "Not found setRangeTemperature");
  }
}

void initServer()
{
Serial.println("entrando en initserver");
  
  server.on("/LED", HTTP_POST, handleFormLed);
  server.on("/prueba", HTTP_GET, handleProof);
  server.on("/getTemperature", HTTP_GET, getTemperature);

  server.on("/setRangeTemperature", HTTP_POST, setRangeTemperature);

  server.onNotFound([](AsyncWebServerRequest * request) {
    request->send(400, "application/json", "Bad request");
  });

  server.begin();
}
