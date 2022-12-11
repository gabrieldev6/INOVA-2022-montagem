// Função que envia dados de consumo pela out.
void Envia_dados_arquivo(){
  File out = SD.open("/octetosOut.csv", FILE_APPEND);
  if(!out){
    Serial.println("Falha ao abrir o arquivos de saída!");
    while(1);
  }
    out.print(tempo_restante);
    out.print(":");
    out.print(ativos_ponta_total);
    out.print(":");
    out.print(ativos_fora_ponta_total);
    out.print(":");
    out.print(reativos_ponta_total);
    out.print(":");
    out.print(reativos_fora_ponta_total);
    out.print(":");

/*    
  //Reposição de demanda
  out.print(" Rep.: ");
  out.print((reposicao?"Sim:":"Não:"));
*/
  // Intervalo de reativos
  out.print((int_reativo?"Sim:":"Não:"));
  
  // Considera reativos
  out.print((considera_capacitivo?"Sim:":"Não:"));  
  
  // Considera indutivos
  out.print((considera_indutivo?"Sim:":"Não:")); 
  
  // Tarifa reaivo
  out.print((tarifa_reativo?"ativada:":"desativada:"));  
  
  switch(tipo)
  {
    case 0: out.println("azul");
    break;
    case 1: out.println("verde");
    break;
    case 2: out.println("irrigantes");
    break;
    case 3: out.println("outras");
    break;
    default: out.println("Não identificada");
  }
/*
  out.print(" Horário: ");
  switch(tarifa){
    case 1: out.println("ponta:");
    break;
    case 2: out.println("fora ponto:");
    break;
    case 8: out.println("reservado:");
    break;
    default:out.println("..:");
  }
  */
  out.close();
}
           
