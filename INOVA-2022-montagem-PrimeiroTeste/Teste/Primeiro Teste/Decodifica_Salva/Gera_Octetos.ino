void Gera_Octetos(){

  if(!file.available()){
    all = true;
  }
  
  int i = 0;
  
  if(file){ //Gera a linha
    int i = 0;
    char item;
     while (file.available()) {
      item = file.read(); 
      if(item != '\n'){
        line[i]=item;
        i++;
      }
      else{
        line[i] = '\n';
        break;
      }
     }
  }
  
  char * octeto = strtok(line, ":"); //Extrai o octeto
  while(octeto != NULL){
    octetos_aux[i] = (byte)String(octeto).toInt();
    //Serial.println(octeto);
    octeto = strtok(NULL, ":");
    i++;
  }
}

 void cleanLine(){
    for(int i =0; i < 30; i++){
      line[i] = ' ';
    }
 }
