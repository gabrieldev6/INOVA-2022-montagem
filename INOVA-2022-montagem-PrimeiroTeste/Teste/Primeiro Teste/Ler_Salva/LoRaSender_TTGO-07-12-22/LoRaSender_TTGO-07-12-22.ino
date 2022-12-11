#include <SPI.h>
#include <LoRa.h>

int counter = 0;

// WIFI_LoRa_32 ports
// GPIO5  -- SX1278's SCK
// GPIO19 -- SX1278's MISO
// GPIO27 -- SX1278's MOSI
// GPIO18 -- SX1278's CS
// GPIO14 -- SX1278's RESET
// GPIO26 -- SX1278's IRQ(Interrupt Request)


#define SCK     5
#define MISO    19
#define MOSI    27
#define CS      18
#define RST     14
#define DI00    26


#define BAND 915E6

void setup() {
  Serial.begin(115200);

  Serial.println("LoRa Sender");

  SPI.begin(SCK,MISO,MOSI,CS);

  
  LoRa.setPins(SS,RST,DI00);
  
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("lora inicializado");
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(1000);
}
