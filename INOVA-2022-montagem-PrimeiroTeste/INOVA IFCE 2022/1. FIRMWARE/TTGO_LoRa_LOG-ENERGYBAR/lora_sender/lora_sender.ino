#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <SSD1306.h>


#define SCK_LoRa     5    // GPIO5  -- SX1278's SCK
#define MISO_LoRa    19   // GPIO19 -- SX1278's MISO
#define MOSI_LoRa    27   // GPIO27 -- SX1278's MOSI
#define SS_LoRa      18   // GPIO18 -- SX1278's CS
#define RST_LoRa     14   // GPIO14 -- SX1278's RESET
#define DI0_LoRa     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)

byte octetos[9]="12345678"; //  Vetor de Bytes que acondiciona os dados recebidos da SSU.
byte oux_byte=0;  // Ou exclusivo de todos os dados recebidos pela SSU até o momento. Ao final, deve-se realizar a inversão desta variável para a comparação com o Byte de verificação.
int counter = 0;
   
SSD1306 display(0x3c, 21, 22);

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
   
  Serial.begin(115200);
  while (!Serial); //If just the the basic function, must connect to a computer

 
  // Inicialização da comunicação SPI.
  SPI.begin(SCK_LoRa,MISO_LoRa,MOSI_LoRa,SS_LoRa);

  // Inicialização do LoRa pela através de SPI.
  LoRa.setPins(SS_LoRa,RST_LoRa,DI0_LoRa);  
  
  if (!LoRa.begin(BAND)) {
    Serial.println("Falhou em iniciar o LoRa!");
    while (1);
  }
  
  LoRa.receive();
  
  Serial.println("Inicialização LoRa OK!");
   
  display.init();
  display.flipScreenVertically();  
  display.setFont(ArialMT_Plain_10);

  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Inicializado!");
  for(int i=0;i<8;i++)
    display.drawString(i*10, 16, (String)octetos[i]);
  //display.drawStringMaxWidth(0 , 26 , 128, packet);
  display.display();
   
  delay(1500);

  // Prepara os dados para envio.
  oux_byte=0;
  for(int i=0;i<7;i++)
    oux_byte^=octetos[i];
  octetos[7]=~oux_byte;
}

int i=0;

void loop() {  
  delay(1000);
  counter++;
  oux_byte=0;
  // send packet LoRa
  LoRa.beginPacket();
LoRa.write(octetos,8);
//  for(int i=0;i<8;i++){
//    LoRa.print(octetos[i],HEX);
//    Serial.wirte(octetos[i],HEX);
//    oux_byte^=octetos[i];
//    Serial.print((String)"|"+oux_byte+" |");
//  }
//  oux_byte=~oux_byte;
//  LoRa.print(oux_byte,HEX);
//  Serial.println(oux_byte,HEX);

  LoRa.endPacket();

}
