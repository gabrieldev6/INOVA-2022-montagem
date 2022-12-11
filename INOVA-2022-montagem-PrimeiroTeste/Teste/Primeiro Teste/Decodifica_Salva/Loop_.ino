void loop() {
  if (!all){
    Gera_Octetos();
    
    data_="";
    oux_byte=0;
    
    for (int i=0; i < 8; i++){
      if(i>7){
        i=0;
        data_="";
        oux_byte=0;
      }
      octetos[i] = octetos_aux[i];
      
      if(i<7)
        oux_byte^=octetos[i];
        
      data_+=(String)octetos[i];
    }
    
    
    // Inverte o byte de verificação.
    oux_byte=~oux_byte;
   
    // DEBUG: Envia os Bytes de verificação recebido e calculado. 
    //Serial.print(octetos[7],HEX);
    //Serial.print(" : ");
    //Serial.print(oux_byte,HEX);
    //Serial.println();   

    /*
    for (int i = 0; i < 8; i++){
      Serial.print(octetos[i], HEX);
    }
    Serial.println();
    */
    
    // Verificação de Bytes: (true) Bytes recebidos com sucesso, (false) descartar Bytes.
    // octetos[7] => último byte recebido.
    // oux_byte => ou exclusivo de todos os bytes recebidos até agora. Tem-se que inverter bit-a-bit o valor de oux_byte antes da verificação.
    verificacao_ok=((octetos[7]==oux_byte)?true:false);
      
    //if(verificacao_ok){
        // Decodifica dados recebisos pela SSU.
        Decodifica();
        // Atualiza o valor da conta.
        Atualiza_valor();
        // Define o tipo de alerta de acordo com o horário atual.
        Alerta(); 
        
        //DEBUG: Envia dados pela Serial para o computador.
        //Envia_dados_serial();
        Envia_dados_arquivo();         
      //}

      //Zera contagem de octetos para nova recepção.  
      indice_octeto=0;
      
      //Limpa o Byte de verificação.
      oux_byte=0;

        // Limpa o buffer de recepção.
      for(int i=0;i<8;i++)
        octetos[i]=0;
    
      //Baixa a flag de recebimento OK.
      verificacao_ok=false;
  
      cleanLine();
  }else{
    Serial.println("End file");
    delay(10000);
  }
}
