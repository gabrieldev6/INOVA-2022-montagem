// Função que envia dados de consumo pela serial.
void envia_dados_serial(){

    Serial.print("[Resta ");
    Serial.print(tempo_restante);
    Serial.print(" segundos :");
    Serial.print(" ativos (ponta: ");
    Serial.print(ativos_ponta_total);
    Serial.print(", fora: ");
    Serial.print(ativos_fora_ponta_total);
    Serial.print(") reativos (ponta: ");
    Serial.print(reativos_ponta_total);
    Serial.print(", fora: ");
    Serial.print(reativos_fora_ponta_total);
    Serial.print(")]");

/*    
  //Reposição de demanda
  Serial.print(" Rep.: ");
  Serial.print((reposicao?"Sim.":"Não."));
*/
  // Intervalo de reativos
  Serial.print(" I. r: ");
  Serial.print((int_reativo?"Sim.":"Não."));
  
  // Considera reativos
  Serial.print(" C. r: ");
  Serial.print((considera_capacitivo?"Sim.":"Não."));  
  
  // Considera indutivos
  Serial.print(", i: ");
  Serial.print((considera_indutivo?"Sim.":"Não.")); 
  
  // Tarifa reaivo
  Serial.print(" T. r: ");
  Serial.print((tarifa_reativo?"ativada.":"desativada."));  

  Serial.print(" Tipo: ");
  switch(tipo)
  {
    case 0: Serial.println("azul.");
    break;
    case 1: Serial.println("verde.");
    break;
    case 2: Serial.println("irrigantes.");
    break;
    case 3: Serial.println("outras.");
    break;
    default: Serial.println("Não identificada.");
  }
/*
  Serial.print(" Horário: ");
  switch(tarifa){
    case 1: Serial.println("ponta.");
    break;
    case 2: Serial.println("fora ponto.");
    break;
    case 8: Serial.println("reservado.");
    break;
    default:Serial.println("...");
  }
  */
}
           
