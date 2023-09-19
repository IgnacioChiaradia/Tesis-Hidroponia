void handleFormLed(AsyncWebServerRequest *request)
{
  String ledStatus = request->arg("status");
  
  Serial.print("Status:\t");
  Serial.println(ledStatus);
  
  digitalWrite(LED_BUILTIN, ledStatus == "ON" ? LOW : HIGH);
  
  AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", "Hello World!");
  response->addHeader("Server","ESP Async Web Server");
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
  Serial.println(actualTemperature);

  AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", actualTemperature);
  //response->addHeader("Temperatura actual ", (String)actualTemperature);

  request->send(response);
  
  request->redirect("/");    
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
