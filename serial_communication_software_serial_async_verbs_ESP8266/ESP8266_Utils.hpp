void connectWifi(){

  // el ESP8266 actuará en modo estación (STA, Station), es decir, como un dispositivo que se conecta a un punto de acceso
  WiFi.mode(WIFI_STA);
  //Wifi connection
  WiFi.begin(net_name, pass); 

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
