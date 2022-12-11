void setup() {
  
  // Configuração OLED
  pinMode(16,OUTPUT);
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50); 

  digitalWrite(16, HIGH);
  /*
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
*/
  // Configuração Serial.
  Serial.begin(115200);
  while (!Serial); //if just the the basic function, must connect to a computer
  delay(1000);
  Serial.println("Init Serial");

  SPI.begin(CLK, MISO, MOSI, CS);
  if (!SD.begin(13)) {
    Serial.println("initialization failed!");
    while (1);
  }

  file = SD.open("/octetos.txt", FILE_READ);
  if(!file){
    Serial.println("Erro ao abrir o arquivo");
    return;
  }
  delay(500);

  if (SD.exists("/octetosOut.csv")){
    SD.remove("/octetosOut.csv");
  }
  if (!SD.exists("/octetosOut.csv")){
    Serial.println("Arquivo apagado com sucesso!");
  }
  delay(2000);
}
