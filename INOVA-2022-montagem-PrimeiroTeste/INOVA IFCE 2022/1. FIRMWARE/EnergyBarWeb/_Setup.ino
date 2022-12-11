void setup() {
  
  Serial.begin(9600);
  while (!Serial);
 
  SSU.begin(110);
  while (!SSU);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  //Serial.print("IP local> ");
  //Serial.println(Ethernet.localIP());

    // Inicializa o bit de reposição desde a última vez que o arduíno esteve ligado.
    EEPROM.get(end_reposicao,reposicao);
    reposicao_anterior=reposicao;
      
  /*
  // Sinaliza a correta inicialização do cartão SD.
  // Caso a função retorne ZERO, a inicialização falhou.
  //inicializou_SD_corretamente=(?true:false);
  SD.begin(4);

  File arquivo=SD.open("enel.txt", FILE_WRITE);
    
  // Grava no cartão SD o cabeçalho do arquivo.
  if(arquivo)
  {
    arquivo.print("Reposição");
    arquivo.print("\t");
    
    arquivo.print("R$");
    arquivo.print("\t");
    
    arquivo.print("Pulsos Ativos");
    arquivo.print("\t");
    
    arquivo.print("Pulsos Reativos");
    arquivo.print("\t");
     
    arquivo.print("Intervalo Reativo");
    arquivo.print("\t");
    
    arquivo.print("Considera Reativo");
    arquivo.print("\t");
    
    arquivo.print("Considera Indutivo");
    arquivo.print("\t");
   
    arquivo.print("Tarifa Reativo");
    arquivo.print("\t");

    arquivo.print("Tarifa");
    arquivo.print("\t");
    
    arquivo.println("Horário");
    
    arquivo.close();
  }
  */
}
