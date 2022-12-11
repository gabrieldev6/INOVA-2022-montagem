// Função que atualiza o valor total da conta de energia.
void Atualiza_valor(){
  //Início de um novo intervalo de integração.
  if(tempo_restante_anterior<tempo_restante)
  {
    tempo_restante_anterior=tempo_restante;
    segundos_intervalo += 3;
  }
  else
  {
    segundos_intervalo += tempo_restante_anterior-tempo_restante;
  }
  
  // A cada fim de intervalo de integração, deve-se totalizar o valor da conta em valor_total.
 if(fim_intervalo)
 { 
    conta_fim_intervalo++;
    // Ao contar 3 vezes o zero segundos de fim de intervalo.
    if(conta_fim_intervalo==3)
    {
      reativos=0;
      reativos_anterior=0;
      ativos=0;
      ativos_anterior=0;

      // Baixa a flag para realizar totalização a cada fim de intervalo de integração de 15 min.
      fim_intervalo=false;
      conta_fim_intervalo=0;
    }
 }

 // Em caso de identificado uma reposição de demanda.
 if(totalizacao) 
 {
  conta_totalizacao++;

  //Baixa a flag de reposição de demanda.
  totalizacao=false;
 }

  // Cálculo do tempo do horário ponta ou fora ponta.
  if(tarifa_mudou)
  {// Houve mudança de tarifa.
      tarifa_mudou=false;
      
      segundos_intervalo=0;
      ativos_horario_atual=0;
      reativos_horario_atual=0;    
  }

          // Calcula o consumo de pulsos ativos no último segundo.
          delta_ativos=ativos-ativos_anterior;
          
          // Calcula o consumo de pulsos reativos no último segundo.
          delta_reativos=reativos-reativos_anterior;

           // Contabiliza os ativos e reativos do horário atual. 
           ativos_horario_atual+=delta_ativos;
           reativos_horario_atual+=delta_reativos;
  
 // Cálculo do valor da conta atual.
 // Valor da conta atual é a soma do valor da conta integralizado até o intervalo (15 min) de demanda anterior, mais o consumido até aquele momento no intervalo atual.
 switch(tarifa)
 {
     // Ponta.
     // Cálculo considerando o intervalo de Ponta: R$1.61/kWh
   case 1: ativos_ponta_total+=delta_ativos;
           reativos_ponta_total+=delta_reativos;
   break;
     // Fora ponta.
     // Cálculo considerando o intervalo Fora Ponta: R$0.41/kWh
   case 2: ativos_fora_ponta_total+=delta_ativos;
           reativos_fora_ponta_total+=delta_reativos;
   break;
 }
  
  // Cálculo do valor da conta até o momento em reais.
  valor_conta=((float)ativos_ponta_total*cte_medicao_kWh_por_pulso*cte_RS_por_kWh_ponta+(float)reativos_ponta_total*cte_medicao_kVAR_por_pulso*cte_RS_por_kVArh_ponta)+
              ((float)ativos_fora_ponta_total*cte_medicao_kWh_por_pulso*cte_RS_por_kWh_fora_ponta+(float)reativos_fora_ponta_total*cte_medicao_kVAR_por_pulso*cte_RS_por_kVArh_fora_ponta);

}
