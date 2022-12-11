void loop() {
  // Verifica se há bytes recebidos pela interface LoRa.
  int packetSize = LoRa.parsePacket();
     
  if (packetSize) {
    // read packet
    data_="";
    oux_byte=0;
    for (int i=0;LoRa.available();i++){
      if(i>7){
        i=0;
        data_="";
        oux_byte=0;
      }
      octetos[i] = LoRa.read();
      
      if(i<7)
        oux_byte^=octetos[i];
        
      data_+=(String)octetos[i];
    }
    
    // Inverte o byte de verificação.
    oux_byte=~oux_byte;
    
    // print RSSI of packet
    Serial.print("Received packet: ");
    for (int i=0;i<8;i++)
      Serial.print(octetos[i],HEX);
    Serial.print(" ");
    Serial.print(" with RSSI ");
    Serial.println(LoRa.packetRssi());
        
    // received a packets
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.drawString(3, 0, data_);
    display.drawString(20, 45, "RSSI:  ");
    display.drawString(70, 45, (String)LoRa.packetRssi());
    display.display();
    
    // DEBUG: Envia os Bytes de verificação recebido e calculado. 
    //Serial.print(octetos[7],HEX);
    //Serial.print(" : ");
    //Serial.print(oux_byte,HEX);
    //Serial.println();

    
    // Verificação de Bytes: (true) Bytes recebidos com sucesso, (false) descartar Bytes.
    // octetos[7] => último byte recebido.
    // oux_byte => ou exclusivo de todos os bytes recebidos até agora. Tem-se que inverter bit-a-bit o valor de oux_byte antes da verificação.
    verificacao_ok=((octetos[7]==oux_byte)?true:false);
      
    if(verificacao_ok){
        // Decodifica dados recebisos pela SSU.
        Decodifica();
        // Atualiza o valor da conta.
        Atualiza_valor();
        // Define o tipo de alerta de acordo com o horário atual.
        Alerta(); 
        
          //DEBUG: Envia dados pela Serial para o computador.
          Envia_dados_serial();         
      }

      //Zera contagem de octetos para nova recepção.  
      indice_octeto=0;
      
      //Limpa o Byte de verificação.
      oux_byte=0;

        // Limpa o buffer de recepção.
      for(int i=0;i<8;i++)
        octetos[i]=0;
    
      //Baixa a flag de recebimento OK.
      verificacao_ok=false;
  }

  // Verifica alguma solicitação de página Web.
  client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            page(); // Envia a página para o solicitante.    
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
