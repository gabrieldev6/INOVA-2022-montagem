#include "DHTesp.h"

// Pino 23: Data
// Vcc:     3.3V
// Gnd:     gnd

DHTesp dht;

/** Pin number for DHT11 data pin */
int dhtPin = 23;


bool inicializa_DHT11(){
  
  informacoes_inicializacao_OLED("Inicializando DHT11...");
  // Initialize temperature sensor
  dht.setup(dhtPin, DHTesp::DHT11);
  
}

bool atualiza_temperatura_umidade() {
  // Reading temperature for humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
  TempAndHumidity newValues = dht.getTempAndHumidity();
  // Check if any reads failed and exit early (to try again).
  if (dht.getStatus() != 0) {
    Serial.println("DHT11 error status: " + String(dht.getStatusString()));
    return false;
  }

  temperatura=newValues.temperature;
  umidade=newValues.humidity;

  return true;
}
