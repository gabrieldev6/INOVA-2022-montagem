void page(){
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");  // the connection will be closed after completion of the response
  client.println("Refresh: 5");  // refresh the page automatically every 5 sec
  client.println();
  
  client.println("<!DOCTYPE html>");
  client.println("<html lang=\"pt-br\">");
  client.println("<head>");
  client.println("<meta charset=\"UTF-8\">");
  client.println("<title>EnergyBar Aracati</title>");
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
  client.println("<div class=\"segura\">EnergyBar - IFCE Aracati</div>");
  client.println("</div>");
                            
  client.println("</div>");
                            
  client.println("</body>");
  client.println("</html>"); 
}

