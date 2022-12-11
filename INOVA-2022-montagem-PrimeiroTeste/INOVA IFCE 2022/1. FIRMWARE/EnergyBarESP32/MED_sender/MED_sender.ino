#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <SSD1306.h>
#include <Arduino.h>
#include <HardwareSerial.h>

byte octetos[9]="12345678"; //  Vetor de Bytes que acondiciona os dados recebidos da SSU.
byte oux_byte=0;  // Ou exclusivo de todos os dados recebidos pela SSU até o momento. Ao final, deve-se realizar a inversão desta variável para a comparação com o Byte de verificação.

String octetos_=" ";

bool verificacao_ok=false;  // Flag que indica verificação de dados vindos da SSU está ok.
bool ocioso=true;   // Flag que sinaliza a não recepção de dados da SSU.

unsigned int tempo=0;  // tempo contato entre dados recebidos da SSU a fim de verificar o timeout. ACREDITO NÃO PRECISAR SER LONG. ANALISAR FUTURAMENTE.
unsigned int indice_octeto=0; // Índice que aponta para próxima região livre do vetor de octetos.

int counter = 0;

HardwareSerial SSU(1);
 
//OLED pins to ESP32 GPIOs via this connecthin:
//OLED_SDA -- GPIO4
//OLED_SCL -- GPIO15
//OLED_RST -- GPIO16
 
SSD1306  display(0x3c, 4, 15);
 
// WIFI_LoRa_32 ports
// GPIO5  -- SX1278's SCK
// GPIO19 -- SX1278's MISO
// GPIO27 -- SX1278's MOSI
// GPIO18 -- SX1278's CS
// GPIO14 -- SX1278's RESET
// GPIO26 -- SX1278's IRQ(Interrupt Request)
 
#define SS      18
#define RST     14
#define DI0     26
#define BAND    433E6  //915E6 
 
void setup() {
  pinMode(25,OUTPUT); //Send success, LED will bright 1 second
  pinMode(16,OUTPUT);
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50); 
  digitalWrite(16, HIGH);

  SSU.begin(110,SERIAL_8N1, 23, 22);
  while (!SSU);
  
  Serial.begin(115200);
  while (!Serial); //If just the the basic function, must connect to a computer
  
  // Initialising the UI will init the display too.
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(5,5,"LoRa Sender");
  display.display();
   
  SPI.begin(5,19,27,18);
  LoRa.setPins(SS,RST,DI0);
  Serial.println("LoRa Sender");
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initial OK!");
  display.drawString(5,20,"LoRa Initializing OK!");
  display.display();
  delay(2000);
}

void envia_lora(){
  // Formação da string.
  octetos_="";
  for(int i=0;i<8;i++)
    octetos_+=octetos[i];
  
  Serial.print(counter);
  Serial.print(": ");
//  for(int i=0;i<8;i++)
//    Serial.write(octetos[i]);
  Serial.print(octetos_);
  Serial.println();
  
  display.clear();
  display.setFont(ArialMT_Plain_16);  
  display.drawString(12, 5, octetos_);
  display.drawString(50, 30, String(counter));
  display.display();
  
  // send packet
  LoRa.beginPacket();
    LoRa.write(octetos,8);
  LoRa.endPacket();
  counter++;
}

void loop() {

    //Timeout: tempo entre dados da SSU excedeu o tempo limite de 50ms. Fim de transmissão SSU.
    //TIMEOUT Observado: 290.0 ms
    //Tempo entre caracteres: 12.0 ms
    //if((unsigned int)millis()-tempo>50.0 && !ocioso)
    if(true)
    {  
      // Verificação de Bytes: (true) Bytes recebidos com sucesso, (false) descartar Bytes.
      // octetos[7] => último byte recebido.
      // oux_byte => ou exclusivo de todos os bytes recebidos até agora. Tem-se que inverter bit-a-bit o valor de oux_byte antes da verificação.
      //verificacao_ok=((octetos[7]==((byte)~oux_byte))?true:false);
      verificacao_ok=true;
      
      if(verificacao_ok)
      {
        envia_lora();
      }
      delay(1000);

      //Zera contagem de octetos para nova recepção.  
      indice_octeto=0;
      
      //Limpa o Byte de verificação.
      oux_byte=0;

        // Limpa o buffer de recepção.
      for(int i=0;i<8;i++)
        octetos[i]=0;
    
      //Baixa a flag de recebimento OK.
      verificacao_ok=false;
      
      // Flag que indica fim de atividade de recebimento de dados.
      ocioso=true;
    }
  
  if(SSU.available())
  {  
    //Flag que indica recebimento de dados da SSU.
    ocioso=false;
    
    byte c = (byte)SSU.read();
    
    //Em caso de ser um dado.
    if(indice_octeto<8)
    {
      //Concatena os bytes recebidos da SSU no vetor octetos.
      octetos[indice_octeto]=c;
    
      if(indice_octeto<7)
      {
        // Faz-se o ou exclusivo dele com os dados recebidos, com exceção do próprio Byte de verificação.
        oux_byte^=c;
      }
      
      // Incrementa posição do índice para a próxima posição disponível.
      indice_octeto++;
     
      // Reinicia a contagem de tempo para avaliação do Timeout.
      tempo=millis();
    }
  } 
}
