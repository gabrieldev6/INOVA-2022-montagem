void Alerta(){
  
    switch(tarifa)
    { //Ponta
      case 1: 
              if(segundos_intervalo<tempo_tolerancia_ponta)
              { // Dentro do tempo de tolerância.
                if(ativos_horario_atual+reativos_horario_atual<Pulso_total_previsto_ponta*porcentagem_tolerancia_alerta_vermelho_ponta*porcentagem_alerta_amarelo){
                  // Consumo menor que o mínimo estipulado para o horário.
                  alerta_consumo=1;
                }
                else{
                  if(ativos_horario_atual+reativos_horario_atual>Pulso_total_previsto_ponta*porcentagem_tolerancia_alerta_vermelho_ponta){
                    // Consumo menor que o máximo estipulado para o horário.
                    alerta_consumo=3;
                  }
                  else{
                    alerta_consumo=2;
                  }
                }
              }
              else // Fora do tempo de tolerância.
              {
                if(ativos_horario_atual+reativos_horario_atual<(Pulso_total_previsto_ponta/Tempo_total_ponta)*segundos_intervalo*porcentagem_alerta_amarelo){
                // Consumo menor que o mínimo estipulado para o horário.
                  alerta_consumo=1;
                }
                else{
                  if(ativos_horario_atual+reativos_horario_atual>(Pulso_total_previsto_ponta/Tempo_total_ponta)*segundos_intervalo){
                  // Consumo menor que o máximo estipulado para o horário.
                    alerta_consumo=3;
                  }
                  else{
                    alerta_consumo=2;
                  }
                }  
              }
      break;
      //Fora ponta
      case 2: 
               if(segundos_intervalo<tempo_tolerancia_fora_ponta)
               { // Dentro do tempo de tolerância.
                if(ativos_horario_atual+reativos_horario_atual<Pulso_total_previsto_fora_ponta*porcentagem_tolerancia_alerta_vermelho_fora_ponta*porcentagem_alerta_amarelo){
                // Consumo menor que o mínimo estipulado para o horário.
                  alerta_consumo=1;
                }
                else{
                  if(ativos_horario_atual+reativos_horario_atual>Pulso_total_previsto_fora_ponta*porcentagem_tolerancia_alerta_vermelho_fora_ponta){
                  // Consumo menor que o máximo estipulado para o horário.
                    alerta_consumo=3;
                  }
                  else{
                    alerta_consumo=2;
                  }
                }
               }
               else
               {
                if(ativos_horario_atual+reativos_horario_atual<(Pulso_total_previsto_fora_ponta/Tempo_total_fora_ponta)*segundos_intervalo*porcentagem_alerta_amarelo){
                // Consumo menor que o mínimo estipulado para o horário.
                  alerta_consumo=1;
                }
                else{
                  if(ativos_horario_atual+reativos_horario_atual>(Pulso_total_previsto_fora_ponta/Tempo_total_fora_ponta)*segundos_intervalo){
                  // Consumo menor que o máximo estipulado para o horário.
                    alerta_consumo=3;
                  }
                  else{
                    alerta_consumo=2;
                  }
                }
               }
      break;
      default: alerta_consumo=3;  // Para o caso de o flag de tarifa estar sendo lido de forma errada.
    }
}
