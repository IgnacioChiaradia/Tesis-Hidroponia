#include "Temperature.h"

// esta variable se usa para guardar desde el main y la devolvemos en la response desde aqui
Temperature temperatureSensor;

void getTemperature(AsyncWebServerRequest *request) {

  // reviso que el sensor este devolviendo una temperatura valida ya que -127.0 significa que hubo un fallo en la lectura
  if(temperatureSensor.get() != "-127.0"){ 
    //por ahora solo usamos el dato de la temperatura y lo enviamos en formato json
    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", temperatureSensor.get());
    response->addHeader("Server message", "getTemperature");
    request->send(response);    
  } else {
    request->send(404, "application/json", "Not found getTemperature");
  }
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

  //GET
  server.on("/getTemperature", HTTP_GET, getTemperature);

  //POST
  server.on("/setRangeTemperature", HTTP_POST, setRangeTemperature);

  server.onNotFound([](AsyncWebServerRequest * request) {
    request->send(400, "application/json", "Bad request");
  });

  server.begin();
}
