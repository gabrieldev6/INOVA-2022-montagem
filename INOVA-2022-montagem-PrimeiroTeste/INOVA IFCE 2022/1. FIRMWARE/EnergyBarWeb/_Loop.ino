void loop() {
       
  //Timeout: tempo entre dados da SSU excedeu o tempo limite de 50ms. Fim de transmissão SSU.
    //TIMEOUT Observado: 290.0 ms
    //Tempo entre caracteres: 12.0 ms
    if((unsigned int)millis()-tempo>50.0 && !ocioso)
    {  

      //DEBUG: Comparação dos códigos de verificação.
      //Serial.print(octetos[7],HEX); Serial.print("=");Serial.println((byte)~oux_byte,HEX);
      
      // Verificação de Bytes: (true) Bytes recebidos com sucesso, (false) descartar Bytes.
      // octetos[7] => último byte recebido.
      // oux_byte => ou exclusivo de todos os bytes recebidos até agora. Tem-se que inverter bit-a-bit o valor de oux_byte antes da verificação.
      verificacao_ok=((octetos[7]==((byte)~oux_byte))?true:false);
      
      if(verificacao_ok)
      {
        // Decodifica dados recebisos pela SSU.
        decodifica();
        // Atualiza o valor da conta.
        atualiza_valor();
        // Define o tipo de alerta de acordo com o horário atual.
        Alerta(); 
        
          //DEBUG: Envia dados pela Serial para o computador.
          //envia_dados_serial();         
      }

      //Zera contagem de octetos para nova recepção.  
      indice_octeto=0;
      
      //Limpa o Byte de verificação.
      oux_byte=0;

        // Limpa o buffer de recepção.
      for(int i=0;i<8;i++)
        octetos[i]=0;
    
      //Baixa a flag de recebimento OK.
      verificacao_ok=0;
      
      // Flag que indica fim de atividade de recebimento de dados.
      ocioso=true;

/*
       // DEBUG: Verificação dos dados recebidos. Em caso de OK! ou Não OK!
      if(verificacao_ok)
        Serial.println("OK!");
      else
        Serial.println("N!");
  */       
     
      //BEBUG: Separação entre os dados recebidos e enviados pela serial.
      //Serial.println();
    }
  
  if(SSU.available())
  {  
    //Flag que indica recebimento de dados da SSU.
    ocioso=false;
    
    byte c = (byte)SSU.read();
    
    //Em caso de ser um dado.
    if(indice_octeto<8)
    {
      //Concatena os bytes recebidos da SSU no vetor octetos.
      octetos[indice_octeto]=c;
    
      if(indice_octeto<7)
      {
        // Faz-se o ou exclusivo dele com os dados recebidos, com exceção do próprio Byte de verificação.
        oux_byte^=c;
      }
      
      // Incrementa posição do índice para a próxima posição disponível.
      indice_octeto++;
     
      // Reinicia a contagem de tempo para avaliação do Timeout.
      tempo=millis();
    }

    // Envia dado recebido pela serial: DEBUG.
    //Serial.print(c,HEX);
  } 

  // listen for incoming clients
  EthernetClient client = server.available();
  
  if (client) {
    //DEBUG: Para saber quando um cliente solicita informação.
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) { 
        char c = client.read();
        //Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();

          client.println("<!DOCTYPE html>");
          client.println("<html lang=\"pt-br\">");
          client.println("<head>");
          client.println("<meta charset=\"UTF-8\">");
          //client.println("<title>Title</title>");
          client.println("<link rel=\"stylesheet\" type=\"text/css\" href=\"https://drive.google.com/uc?export=download&id=0BxNaccCVtXTUdXFHTm44emhNcEk\"/>");
          client.println("</head>");
          client.println("<body>");
          client.println("<div class=\"container\">");
          client.println("<div class=\"cabecalho\">");
          client.println("<h1>Consumo Energético</h1>");
          
          // Para os casos de alerta de consumo 1: verde; 2: amarelo e 3: vermelho.
          switch(alerta_consumo)
          {
            case 1:client.println(" <img class=\"logotipo\" src=\"https://drive.google.com/uc?export=download&id=0BxNaccCVtXTUWDNjcExNajZRU1E\" />");
            break;
            case 2:client.println(" <img class=\"logotipo\" src=\"https://drive.google.com/uc?export=download&id=0BxNaccCVtXTUaS1aZDBKNm9xU0E\" />");
            break;
            case 3:client.println(" <img class=\"logotipo\" src=\"https://drive.google.com/uc?export=download&id=0BxNaccCVtXTUcVVWZmc4Q0RCZ3c\" />"); 
            break;
            default:client.println(" <img class=\"logotipo\" src=\"https://drive.google.com/uc?export=download&id=0BxNaccCVtXTUcVVWZmc4Q0RCZ3c\" />"); 
          }
                                                   
          client.println("</div>");
          client.println("<div class=\"corpo\">");
          client.println("<div class=\"legenda\">");
          client.println("<div class=\"circulo verd\"></div> Dentro da média |");
          client.println("<div class=\"circulo amar\"></div> Limite da média |");
          client.println("<div class=\"circulo verm\"></div> Acima da média");
          client.println("</div>");                        
          client.println("<h1>");
                    client.println("Valor atual da conta: </br>R$"+String(valor_conta,2) );
                    client.println("</h1>");
          client.println("<p>");
                   client.print("Energia ativa (ponta): ");
                   client.print(ativos_ponta_total);
                   client.println(" pulsos<br/>");
                   client.print("Energia ativa (fora ponta): ");
                   client.print(ativos_fora_ponta_total);
                   client.println(" pulsos<br/>");
                   client.print("Energia reativa (ponta): ");
                   client.print(reativos_ponta_total);
                   client.println(" pulsos<br/>");
                   client.print("Energia reativa (fora ponta): ");
                   client.print(reativos_fora_ponta_total);
                   client.println(" pulsos<br/>");
                   client.print("Reposições de demanda: ");
                   client.print(conta_totalizacao);
                   client.println("<br/>");
                   client.print("Tarifa: ");
                                    //<!--Inicio tarifa-->
                                                    switch(tarifa)
                                                     {
                                                      case 1: client.print("ponta.");
                                                      break;
                                                      case 2: client.print("fora ponta.");
                                                      break;
                                                      case 8: client.print("reservada.");
                                                      break;
                                                      default: client.print("?");
                                                    }
                                   // <!--Final tarifa-->
                    client.println("</p>");
          client.println("</div>");
          client.println("<div class=\"rodape\">");
          client.println("<div class=\"barra1 esq\"></div>");
          client.println("<img src=\"https://drive.google.com/uc?export=download&id=0BxNaccCVtXTUV2dVY1NSMmNkZW8\" class=\"logobase\"/>");
          client.println("<div class=\"barra1 dir\"></div>");
          client.println("<div class=\"segura\">EnergyBar - PIBITI 1025/IFCE<br/>Orientador: Valter Costa<br/>Orientando: Weverton Canniggia<br/>Arte-finalista: Fabrício Castelo<br/>Front-end por Emannuel Diego</div>");
          client.println("</div>");
                                    
          client.println("</div>");
                                    
          client.println("</body>");
          client.println("</html>");     
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
}
