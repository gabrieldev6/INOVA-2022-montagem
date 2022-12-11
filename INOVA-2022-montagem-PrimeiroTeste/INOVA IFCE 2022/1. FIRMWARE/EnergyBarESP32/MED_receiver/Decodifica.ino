// Função que decodifica os dados recebidos da SSU e atualiza os valores das variáveis.
void Decodifica(){

tempo_restante_anterior=tempo_restante;

// Tempo: Octeto 1 e 2 (4 últimos bits).
tempo_restante=(unsigned int)(octetos[1]&0x0F);
tempo_restante=tempo_restante<<8;
tempo_restante=tempo_restante|(unsigned int)octetos[0];

// 4 primeiros bits do Octeto 2
// Bit que indica que fechou a totalização do mês. É complementado a cada fechamento.
reposicao=(octetos[1]&0x80?true:false);

// Indica que o intervalo teve consumo de reativos (capacitivo ou indutivo). É complementado ao fim de cada intervalo que é medido algum reativo.
int_reativo=(octetos[1]&0x40?true:false);

// Caso seja 1, indica que o consumo reativo capacitivo será considerado para cálculo de consumo e demanda: UFER (Unidade de Faturamento de Energia Reativa) e DMCR (Demanda Máxima Corrigida).
considera_capacitivo=(octetos[1]&0x20?true:false);

// Caso seja 1, indica que o consumo reativo indutivo será considerado para cálculo de consumo e demanda: UFER (Unidade de Faturamento de Energia Reativa) e DMCR (Demanda Máxima Corrigida).
considera_indutivo=(octetos[1]&0x10?true:false);

// Bits do octeto 3

// Indica o tipo de tarifa que o consumidor está sujeita:
// 00-Azul < De acordo com dados da SSU, o medidor está configurado nesse tipo de tarifa.
// 01-Verde < De acordo com o prof. Tavares: estamos sujeitos a esta tarifa. Demanda fixa e consumo dependendo do horário.
// 10-Irrigantes
// 11-Outras
tipo=(unsigned short)(octetos[2]&0x30);

tarifa_anterior=tarifa;

// OBS: 
// Para cáculo do faturamento de energia, considera-se, ainda, os meses do ano:
// maio a novembro: período seco.
// dezembro a abril: período úmido.
// Em cada período desses o valor cobrado por unidade de energia ativa e reativa, dentro e fora do horário de ponta, deverá ser diferente.

// Inidica o segmento horo-sazonal atual:
// 0001 - ponta
// 0010 - fora de ponta
// 1000 - reservado
// O período que corresponde de 17h a 22h, de segunda a sexta, é considerado ponta.
// O resto é fora ponta.
tarifa=(unsigned short)(octetos[2]&0x0F);

// Se 1, indica que a tarifa de reativo está ativada.
tarifa_reativo=(octetos[2]&0x80?true:false);

// Guarda o valor de ativos da medição do segundo anterior para cálculo do valor da conta.
ativos_anterior=ativos;

// Octetos 4 e 5.
// Pulsos ativos
ativos=(unsigned int)octetos[4];
ativos<<=8;
ativos|=(unsigned int)octetos[3];

// Guarda o valor de reativos da medição do segundo anterior para cálculo do valor da conta.
reativos_anterior=reativos;

// Octetos 6 e 7.
// Pulsos reativos
reativos=(unsigned int)octetos[6];
reativos<<=8;
reativos|=(unsigned int)octetos[5];  

  //Flags
  
  // Levanta a flag de reposição de demanda.
  if(reposicao_anterior!=reposicao)
  {// Guarda o valor do bit que indica a reposição de demanda.
    reposicao_anterior=reposicao;
    // AQUI PÕE ALGUMA ROTINA PARA GUARDAR O VALOR DIÁRIO.
    totalizacao=true;
  }
  
  // Levanta a flag de fim de intervalo.
  if(!tempo_restante)
  {
    fim_intervalo=true;
  }
  // Levanta a flag de horário mudou, com ele a tarifação também.
  // Ponta ou Fora Ponta.

    if(tarifa!=tarifa_anterior)
  {// Houve mudança de tarifa.
    tarifa_mudou=true;   
  }
}
