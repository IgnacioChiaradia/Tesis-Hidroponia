//iniciar cliente
HTTPClient http;
WiFiClient wifiClient;

void connectWifi() {

  // el ESP8266 actuará en modo estación (STA, Station), es decir, como un dispositivo que se conecta a un punto de acceso
  WiFi.mode(WIFI_STA);
  //Wifi connection - cambiar conexion en caso de cambiar de wifi
  WiFi.begin(NET_NAME2, PASS2);

  Serial.print("\nConnecting wifi");

  // Wait for the WifI connection completion
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Conectado a:\t");
  Serial.println(WiFi.SSID());
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
  Serial.print("ESP Mac Address:\t");
  Serial.println(WiFi.macAddress());
}

/*funcion para el seteo inicial del rango al desconectar y conectar el arduino UNO*/
void setInitialRange(){
  if (http.begin(wifiClient, URL_GET_RANGE_TEMPERATURE_FIREBASE)) //Iniciar conexión
  {
    Serial.print("[HTTP] GET...\n");
    int httpCode = http.GET();  // Realizar petición

    if (httpCode > 0) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();  // Obtener respuesta
        Serial.println(payload);  // Mostrar respuesta por serial

        //JsonDocument doc;
        StaticJsonDocument<128> doc;
        deserializeJson(doc, payload);

        const char* MIN_TEMPERATURE = doc["minTemperature"];
        const char* MAX_TEMPERATURE = doc["maxTemperature"];

        Serial.println("temperatura MINIMA " + (String)MIN_TEMPERATURE + "°C y MAXIMA " + (String)MAX_TEMPERATURE + "°C" );

        if (arduinoSerial.available()) {
          //ENVIO DE MAXTEMP y MINTEMP a UNO, usamos minTemperature y maxTemperature como keys
          Serial.println("ENVIO INFO AL ARDUINO UNO PARA CONFIGURACION INICIAL DE RANGO");
          arduinoSerial.print("minTemperature:" + (String)MIN_TEMPERATURE + "maxTemperature:" + (String)MAX_TEMPERATURE);
        }
      }
    }
    else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
  else {
    Serial.printf("[HTTP} Unable to connect\n");
  }
}
