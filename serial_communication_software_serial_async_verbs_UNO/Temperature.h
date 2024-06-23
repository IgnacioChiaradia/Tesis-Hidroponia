// Clase encargada de las acciones referidas a la temperatura
#ifndef TEMPERATURE_H
#define TEMPERATURE_H

class Temperature {

private:
  String temperature = "0.0"; // Valor inicial
  String maxTemp = "0.0";
  String minTemp = "0.0";
  
public:
  String get() {
    return temperature;
  }

  void set(String value) {
    temperature = value;
  }
    
  String getMaxTemp() {
    return maxTemp;
  }

  void setMaxTemp(String value) {
    maxTemp = value;
  }

  String getMinTemp() {
    return minTemp;
  }

  void setMinTemp(String value) {
    minTemp = value;
  }
};

#endif
