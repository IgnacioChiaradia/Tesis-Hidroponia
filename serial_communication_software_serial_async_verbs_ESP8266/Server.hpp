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

  //por ahora solo usamos el dato de la temperatura y lo enviamos en formato json
  AsyncWebServerResponse *response = request->beginResponse(200, "application/json", temperatureSensor.get());
  response->addHeader("Server message", "getTemperature");
  request->send(response);

  //jsonBuffer.clear();

  //request->redirect("/");
}

//SET TEMPERATURA MAXIMA
void setMaxTemperature(AsyncWebServerRequest *request) {

  Serial.print("Mostrando tempMax pedida: ");

  if (request->hasParam("temperature")) {
    AsyncWebParameter* p = request->getParam("temperature");

    temperatureSensor.setMaxTemp(p->value().c_str());
    String maxTemperature = p->value().c_str();

    Serial.println(maxTemperature);

    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", maxTemperature);
    response->addHeader("Server message", "FUNCIONA_el_POST_maxTemperature");
    request->send(response);

    //ENVIO DE MAXTEMP a UNO, usamos maxTemperature como key
    arduinoSerial.print("maxTemperature:" + temperatureSensor.getMaxTemp());

    //request->send(200, "application/json", "FUNCIONA el POST maxTemperature");
  } else {
    request->send(400, "application/json", "Not found setMaxTemperature");
  }
}

//SET TEMPERATURA MINIMA
void setMinTemperature(AsyncWebServerRequest *request) {

  Serial.print("Mostrando tempMin pedida: ");

  if (request->hasParam("temperature")) {
    AsyncWebParameter* p = request->getParam("temperature");

    temperatureSensor.setMinTemp(p->value().c_str());
    String minTemperature = p->value().c_str();

    Serial.println(minTemperature);

    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", minTemperature);
    response->addHeader("Server message", "FUNCIONA_el_POST_minTemperature");
    request->send(response);

    //ENVIO DE MINTEMP al UNO, usamos minTemperature como key
    arduinoSerial.print("minTemperature:" + temperatureSensor.getMinTemp());
    
  } else {
    request->send(400, "application/json", "Not found setMinTemperature");
  }
}

void initServer()
{
Serial.println("entrando en initserver");
  
  server.on("/LED", HTTP_POST, handleFormLed);
  server.on("/prueba", HTTP_GET, handleProof);
  server.on("/getTemperature", HTTP_GET, getTemperature);
  server.on("/setMaxTemperature", HTTP_POST, setMaxTemperature);
  server.on("/setMinTemperature", HTTP_POST, setMinTemperature);

  server.onNotFound([](AsyncWebServerRequest * request) {
    request->send(400, "application/json", "Bad request");
  });

  server.begin();
}
