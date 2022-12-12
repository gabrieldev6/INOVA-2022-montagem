void SaveSD(){
  
  SPI.begin(CLK, MISO, MOSI, CS);
  while(!SD.begin(13)) {
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
}
