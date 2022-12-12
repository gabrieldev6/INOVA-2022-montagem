#include <SPI.h>
#include <LoRa.h>

int counter = 0;

// WIFI_LoRa_32 ports
#define SCK     5   // GPIO5  -- SX1278's SCK
#define MISO    19  // GPIO19 -- SX1278's MISO
#define MOSI    27  // GPIO27 -- SX1278's MOSI
#define CS      18  // GPIO18 -- SX1278's CS
#define RST     14  // GPIO14 -- SX1278's RESET
#define DI00    26  // GPIO26 -- SX1278's IRQ(Interrupt Request)

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
