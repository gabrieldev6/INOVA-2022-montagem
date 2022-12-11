void SaveSD(){
  String octeto="";
  
  file = SD.open("/octecos.txt", FILE_APPEND);
  Serial.print("Save ");
  
  for(int i=0;i<8;i++){
      file.print(octetos[i]);
      file.print(":");  
  }
  file.println();
  
  file.close();  
  Serial.print(": save End\n");
}
