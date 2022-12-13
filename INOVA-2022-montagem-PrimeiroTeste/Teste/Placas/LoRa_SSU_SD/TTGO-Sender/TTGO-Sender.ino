#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <SD.h>
#include <SSD1306.h>
#include <HardwareSerial.h>
#include "RTCDS1307.h"

byte octetos[9]="12345678"; //  Vetor de Bytes que acondiciona os dados recebidos da SSU.
byte oux_byte=0;  // Ou exclusivo de todos os dados recebidos pela SSU até o momento. Ao final, deve-se realizar a inversão desta variável para a comparação com o Byte de verificação.

String octetos_=" ";

bool verificacao_ok=false;  // Flag que indica verificação de dados vindos da SSU está ok.
bool ocioso=true;   // Flag que sinaliza a não recepção de dados da SSU.

unsigned int tempo=0;  // tempo contato entre dados recebidos da SSU a fim de verificar o timeout. ACREDITO NÃO PRECISAR SER LONG. ANALISAR FUTURAMENTE.
unsigned int indice_octeto=0; // Índice que aponta para próxima região livre do vetor de octetos.

int counter = 0;

HardwareSerial SSU(1);

File file;

//pinagem sd
#define SD_MOSI 15
#define SD_MISO 2
#define SD_CLK  14
#define SD_CS   13
//Alterado Carlos 22/11/22 


// Alterado 29/11/22
// SSD1306  display(0x3c, 22, 21);

RTCDS1307 rtc(0x68);

//pinagem lora
#define LORA_SCK     5  // GPIO5  -- SX1278's SCK
#define LORA_MISO    19 // GPIO19 -- SX1278's MISO
#define LORA_MOSI    27 // GPIO27 -- SX1278's MOSI
#define LORA_CS      18 // GPIO18 -- SX1278's CS
#define LORA_RST     14 // GPIO14 -- SX1278's RESET
#define LORA_DI00    26 // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND         915E6  //915E6 

//RTC
uint8_t year, month, weekday, day, hour, minute, second;
bool period = 0;

String timestemp = "";
void SaveSD();
void dateNow();
void envia_lora();

void setup() {
  pinMode(25,OUTPUT); //Send success, LED will bright 1 second
  pinMode(16,OUTPUT);
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50); 
  digitalWrite(16, HIGH);

 Serial.begin(115200);
 
  SSU.begin(110,SERIAL_8N1, 23, 22);
  while (!SSU);
  
  Serial.println("LoRa Initial OK!");
  //inicia porta cartao fecha a porta do lora e 
  SPI.end();

  Serial.println("Init SD");
  delay(1000);
  
  Serial.begin(115200);
  rtc.begin();
  //rtc.setDate(22, 12, 12);
  //rtc.setTime(16, 30, 00);
  
  SD.remove("/octetos.csv");
  if(SD.exists("/octetos.csv")){
    Serial.println("Falha em apagar o arquivo!");
    while(1);
  }
  SPI.end();
}

void loop() {
    // DEBUG dateNow();
    //Timeout: tempo entre dados da SSU excedeu o tempo limite de 50ms. Fim de transmissão SSU.
    //TIMEOUT Observado: 290.0 ms
    //Tempo entre caracteres: 12.0 ms
    if((unsigned int)millis()-tempo>50.0 && !ocioso)
    //if(true)
    {  
      // Verificação de Bytes: (true) Bytes recebidos com sucesso, (false) descartar Bytes.
      // octetos[7] => último byte recebido.
      // oux_byte => ou exclusivo de todos os bytes recebidos até agora. Tem-se que inverter bit-a-bit o valor de oux_byte antes da verificação.
      verificacao_ok=((octetos[7]==((byte)~oux_byte))?true:false);
      //verificacao_ok=true;
      
      if(verificacao_ok)
      {
        SaveSD();
        envia_lora();
      }
      //DEBUG 
      Serial.print(timestemp);
      Serial.print(";");
      Serial.println(octetos_);
      //Serial.println("Pacote enviado!");
      
      SPI.end();
      //delay(1000);

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

      dateNow(); // Quando finalizar a leitura
    }
  } 
}

void SaveSD(){
  
  SPI.begin(SD_CLK, SD_MISO, SD_MOSI, SD_CS);
  while(!SD.begin(SD_CS)) {
    Serial.println("sd nao iniciado");
    delay(1000);
  }
  String octeto="";
  
  file = SD.open("/octetos.csv", FILE_APPEND);

  //erial.println(timestemp);
  file.print(timestemp);
  file.print(",");
  
  for(int i=0;i<8;i++){
      file.print(octetos[i]);
      file.print(";");  
  }
  file.println();
  
  file.close();  
  Serial.print("Save!\n");

  SPI.end();
}

void envia_lora(){
  
  //inicia pinagem para lora
  SPI.begin(LORA_SCK,LORA_MISO,LORA_MOSI,LORA_CS);
  LoRa.setPins(LORA_CS,LORA_RST,LORA_DI00);
  
  Serial.println("LoRa Sender");
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initial OK!");
  
  // Formação da string.
  octetos_="";
  for(int i=0;i<8;i++)
    octetos_+=octetos[i];
  
  Serial.print(counter);
  Serial.print(": ");
  Serial.print(octetos_);
  Serial.println();

  // send packet
  LoRa.beginPacket();
    LoRa.print(timestemp);
    LoRa.print(";");
    LoRa.print(octetos_);
  LoRa.endPacket();
  counter++;

  SPI.end();
}