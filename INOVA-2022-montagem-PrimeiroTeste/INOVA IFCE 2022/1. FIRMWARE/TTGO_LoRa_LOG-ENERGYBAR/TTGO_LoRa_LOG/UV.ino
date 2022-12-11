#include "Adafruit_SI1145.h"
// Vcc: +3.3V 
// Gnd: gnd
// SCL: 21
// SDA: 22

Adafruit_SI1145 uv = Adafruit_SI1145();

void inicializa_UV(){

  informacoes_inicializacao_OLED("Inicializando UV...");

  // Inicializando Sensor UV.
  Serial.println("Initializing UV...");
  
    while(! uv.begin()){
      Serial.println("Não achou Si1145: SCL - pino 21, SDA - pino 22.");
      informacoes_inicializacao_OLED("UV falhou...");
      delay(1000);
    }

  // Inicializando Sensor UV.
  Serial.println("UV OK!");
}

void atualiza_UV(){
  
  // Luz visivel e ir não são calibrados.
  luz_visivel=uv.readVisible();
  luz_ir = uv.readIR();
  
  indice_uv= uv.readUV();
  
  // the index is multiplied by 100 so to get the
  // integer index, divide by 100!
  indice_uv /= 100.0;  

  //Serial.print("Vis: "); Serial.println(luz_visivel);
  //Serial.print("IR: "); Serial.println(luz_ir);
  //Serial.print("UV: ");  Serial.println(indice_uv); 
}
