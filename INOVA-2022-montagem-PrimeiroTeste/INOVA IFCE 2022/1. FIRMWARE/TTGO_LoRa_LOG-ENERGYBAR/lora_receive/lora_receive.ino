#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>  
#include <SSD1306.h>

#define SCK_LoRa     5    // GPIO5  -- SX1278's SCK
#define MISO_LoRa    19   // GPIO19 -- SX1278's MISO
#define MOSI_LoRa    27   // GPIO27 -- SX1278's MOSI
#define SS_LoRa      18   // GPIO18 -- SX1278's CS
#define RST_LoRa     14   // GPIO14 -- SX1278's RESET
#define DI0_LoRa     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)

#define BAND    433E6

SSD1306 display(0x3c, 21, 22);
String rssi = "RSSI --";
String packSize = "--";
String packet ;


void loraData(){
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0 , 15 , "Received "+ packSize + " bytes");
  display.drawStringMaxWidth(0 , 26 , 128, packet);
  display.drawString(0, 0, rssi); 
  display.display();
  Serial.println(rssi);
}

void cbk(int packetSize) {
  packet ="";
  packSize = String(packetSize,DEC);
  for (int i = 0; i < packetSize; i++) { packet += (char) LoRa.read(); }
  rssi = "RSSI " + String(LoRa.packetRssi(), DEC) ;
  loraData();
}

void setup() {
  pinMode(16,OUTPUT);
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50); 
  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high、
  
  Serial.begin(115200);
  while (!Serial);
  
  Serial.println();
  Serial.println("LoRa Receiver Callback");
  
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
  //for(int i=0;i<8;i++)
  //  display.drawString(i*10, 16, (String)octetos[i]);
  //display.drawStringMaxWidth(0 , 26 , 128, packet);
  display.display();
   
  delay(1500);
  
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) { 
    cbk(packetSize);
  }
  delay(10);
}
