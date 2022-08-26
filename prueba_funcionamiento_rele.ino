
#define RELE_ON 0     // Activamos el rele, ponemos valor 0 ya que los reles se activan con nivel bajo o 0 por logica inversa
#define RELE_OFF 1    // Desactivamos el rele, ponemos valor 1 ya que los reles se desactivan con nivel alto o 1 por logica inversa

// definimos pin digital a tomar 
int rele = 2;

void setup() {
    pinMode(rele, OUTPUT);  //configuro rele como salida
    Serial.begin(9600);     //Abrimos consola serial
}

void loop() {
  digitalWrite(rele, RELE_ON);  //envia señal baja, por logica inversa activo RELE
  Serial.println("El rele se encuentra encendido"); // imprimo por consola estado del RELE
  delay(3000); // seteo un tiempo de 3 segundos
  
  digitalWrite(rele, RELE_OFF);   //envia señal alta, por logica inversa desactivo RELE
  Serial.println("El rele se encuentra apagado");
  delay(3000);  
}
