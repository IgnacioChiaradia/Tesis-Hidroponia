void handleFormLed(AsyncWebServerRequest *request)
{
  String ledStatus = request->arg("status");
  
  Serial.print("Status:\t");
  Serial.println(ledStatus);
  
  digitalWrite(LED_BUILTIN, ledStatus == "ON" ? LOW : HIGH);
  
  AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", "Hello World!");
  response->addHeader("Server","ESP Async Web Server");
  response->addHeader("Agradecimientos","a Pelozo");
  request->send(response);
  
  request->redirect("/");
}

void handleProof(AsyncWebServerRequest *request)
{
  Serial.print("Entra a handleProof\t");  
  
  request->send(200, "text/plain", "Funciona el GET");
}

void getTemperature(AsyncWebServerRequest *request){

  Serial.println("Recuperando temperatura actual:\t");

  AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", "Temperatura actual!");
  response->addHeader("Temperatura actual ", (String)actualTemperature);

  request->send(response);
  
  request->redirect("/");  

  // se puede acceder desde aca al arduino UNO ?
  // hacer una clase con tempActual, maxima y minima o variable global ?

  // si es una variable global se guarda desde el main y la devolvemos en la response desde aca

  //ver tema temp maxima y minima

  
}

void initServer()
{
  server.on("/LED", HTTP_POST, handleFormLed);
  server.on("/prueba", HTTP_GET, handleProof);
  server.on("/getTemperature", HTTP_GET, getTemperature);
  
  server.onNotFound([](AsyncWebServerRequest *request) {
    request->send(400, "text/plain", "Not found");
  });
  
  server.begin();
}
