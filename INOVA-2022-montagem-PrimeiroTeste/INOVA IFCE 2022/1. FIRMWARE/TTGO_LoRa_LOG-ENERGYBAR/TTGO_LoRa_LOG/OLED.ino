#include "SSD1306.h" 

SSD1306 display(0x3c, 21, 22);

// Altura e largura da tela OLED.
#define altura_OLED 64
#define largura_OLED 128

void inicializa_OLED(){
  pinMode(16,OUTPUT);
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50); 
  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high、

  display.init();
  display.flipScreenVertically();  
  display.setFont(ArialMT_Plain_10);

  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Inicializando.");
  //display.drawStringMaxWidth(0 , 26 , 128, packet);
  display.display();
   
  delay(1500);
}

void informacoes_LORA(){
  // received a packets
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.drawString(3, 0, data_);
    display.drawString(20, 45, "RSSI:  ");
    display.drawString(70, 45, (String)LoRa.packetRssi());
    display.display();
}

void informacoes_hora_data_OLED(int coluna, int linha){
    display.drawString(coluna,linha, (String)hora +":"+ minuto +" | "+ dias +"/"+ mes +"/"+ anos);
}

void informacoes_pacote_recebido_LORA(int coluna, int linha){
    display.drawString(coluna,linha, data_);
}

void informacoes_contagem_pacote_recebido_LORA(int coluna, int linha){
    display.drawString(coluna,linha, (String)"Pacotes: "+cont);
}

void informacoes_temperatura_umidade(int coluna, int linha){
    display.drawString(coluna,linha, (String)"T "+temperatura+"°C | H "+umidade+"%");
}

void informacoes_LDR(int coluna, int linha){
    display.drawString(coluna,linha, (String)"LDR "+LDR);
}

void informacoes_indice_uv(int coluna, int linha){
    display.drawString(coluna,linha, (String)"UV Index: "+indice_uv);
}

void informacoes_luz_visivel(int coluna, int linha){
    display.drawString(coluna,linha, (String)"Luz V.: "+luz_visivel);
}

void informacoes_luz_ir(int coluna, int linha){
    display.drawString(coluna,linha, (String)"Luz Ir: "+luz_ir);
}

void informacoes_arquivo_SD(int coluna, int linha){
 
    display.drawString(coluna,linha, logFileName);
}

void informacoes_inicializacao_OLED(String informacao){
  // received a packets
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.drawString(0,32, informacao);
    display.display();
}

void atualiza_TELA_OLED(){
  display.clear();
  display.setFont(ArialMT_Plain_16);
  //informacoes_pacote_recebido_LORA(0, 0);
  informacoes_hora_data_OLED(0,0);
  informacoes_arquivo_SD(0,17);
  informacoes_contagem_pacote_recebido_LORA(0,33);
  //informacoes_temperatura_umidade(0,49);
  //informacoes_LDR(0,49);
  //informacoes_indice_uv(0,17);
  //informacoes_luz_visivel(0,33);
  informacoes_luz_ir(0,49);
  display.display();
}

