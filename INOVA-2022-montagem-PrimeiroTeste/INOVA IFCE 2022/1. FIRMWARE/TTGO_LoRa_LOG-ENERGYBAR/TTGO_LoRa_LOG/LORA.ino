#include <LoRa.h>

#define SCK_LoRa     5    // GPIO5  -- SX1278's SCK
#define MISO_LoRa    19   // GPIO19 -- SX1278's MISO
#define MOSI_LoRa    27   // GPIO27 -- SX1278's MOSI
#define SS_LoRa      18   // GPIO18 -- SX1278's CS
#define RST_LoRa     14   // GPIO14 -- SX1278's RESET
#define DI0_LoRa     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)

#define BAND    433E6

byte oux_byte=0;  // Ou exclusivo de todos os dados recebidos pela SSU até o momento. Ao final, deve-se realizar a inversão desta variável para a comparação com o Byte de verificação.

String packet="";
String data_="";   // Recebe os dados do pacote LoRa para o display.

long cont=0;

void inicializa_LORA(){
  informacoes_inicializacao_OLED("Inicializando LoRa...");
  SPI.end();
  // Inicialização da comunicação SPI.
  SPI.begin(SCK_LoRa,MISO_LoRa,MOSI_LoRa,SS_LoRa);

  // Inicialização do LoRa pela através de SPI.
  LoRa.setPins(SS_LoRa,RST_LoRa,DI0_LoRa);  
  
  if (!LoRa.begin(BAND)) {
    Serial.println("Falhou em iniciar o LoRa!");
    informacoes_inicializacao_OLED("Falhou LoRa!");
    while (1);
  }
  
  LoRa.receive();
  
  Serial.println("Inicialização LoRa OK!");
}

void setLoRa_SPI(){
  // Para a comunicação SPI.
  SPI.end();
  // Inicialização da comunicação SPI para o módulo LoRa.
  SPI.begin(SCK_LoRa,MISO_LoRa,MOSI_LoRa,SS_LoRa);
}

bool recepcao_LORA(){
 // Verifica se há bytes recebidos pela interface LoRa.
  int packetSize = LoRa.parsePacket();
     
  if (packetSize) {
      //Zera contagem de octetos para nova recepção.  
      indice_octeto=0;
      
      //Limpa o Byte de verificação.
      oux_byte=0;

      // Limpa o buffer de recepção.
      for(int i=0;i<8;i++)
        octetos[i]=0;
        
      // read packet
      data_="";

    for (int i=0;LoRa.available();i++){
      if(i>7){
        i=0;
        data_="";
        oux_byte=0;
      }
      octetos[i] = LoRa.read();
      
      if(i<7)
        oux_byte^=octetos[i];
        
      data_+=(String)octetos[i];
    }
    
    // Inverte o byte de verificação.
    oux_byte=~oux_byte; 

    // DEBUG: Envia os Bytes de verificação recebido e calculado. 
    //for(int i=0;i<8;i++){
    //  Serial.print(octetos[i],HEX);
    //  Serial.print(" ");
    //}
    //Serial.print(": ");
    //Serial.print(oux_byte,HEX);
    //Serial.println();
    
    // Verificação de Bytes: (true) Bytes recebidos com sucesso, (false) descartar Bytes.
    // octetos[7] => último byte recebido.
    // oux_byte => ou exclusivo de todos os bytes recebidos até agora. Tem-se que inverter bit-a-bit o valor de oux_byte antes da verificação.

    if(octetos[7]==oux_byte){
        // Conta mais um pacote recebido.
        cont++;

          //DEBUG: Envia dados pela Serial para o computador.
          //Envia_dados_serial();
        
        // Recepção LoRa OK!
        return true;         
      }      
    }
  // Retorna falso se o pacote recebido estiver corrompido.
  // Ou não tenhamos recebido nenhum pacote.
  return false;
}

void informacoes_lora_SERIAL(){
  // print RSSI of packet
    Serial.print("Received packet: ");
    for (int i=0;i<8;i++)
      Serial.print(octetos[i],HEX);
    Serial.print(" ");
    Serial.print(" with RSSI ");
    Serial.println(LoRa.packetRssi());
}

