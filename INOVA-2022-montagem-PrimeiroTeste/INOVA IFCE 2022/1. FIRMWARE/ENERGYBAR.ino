// Constantes que definem a quantidade de pulsos consumidos totais previstos para entrar na região amarela (atenção - consumo próximo à margem superior da meta).
#define Pulso_total_previsto_ponta 1875 // Dentro do horário de ponta. 1875 pulsos equivale à kWh.
#define Pulso_total_previsto_fora_ponta 9375 // Fora do horário de ponta. 9375 pulsos equivale à  kWh.
#define tempo_tolerancia_ponta  45*60 // Tempo de tolerância em segundos no início de cada horário ponta: 45 minutos.
#define tempo_tolerancia_fora_ponta 3*60*60  // Tempo de tolerância em segundos no início de cada horário fora ponta: 3 horas.
#define Tempo_total_ponta 3*60*60 // 3 horas em segundos.
#define Tempo_total_fora_ponta 21*60*60 // 21 horas em segundos.
#define porcentagem_alerta_amarelo 0.80  // O valor que indica o limite de indicação de alerta amarelo - 80% do valor máximo definido para o horário.
#define porcentagem_tolerancia_alerta_vermelho_ponta 0.25
#define porcentagem_tolerancia_alerta_vermelho_fora_ponta 0.15  

#define cte_medicao 0.144 // Constante de medição do medidor do campus IFCE Cedro.
                          // Informada pela Enel para a conversão de pulsos em Wh (Fórmulas: Energia_kWh=(144.0/1000)*pulsos_ativos; Energia_kVArh=(144.0/1000)*pulsos_reativos).
#define cte_medicao_kWh_por_pulso 0.144 // Valor certificado para o medidor do Campus Cedro.
#define cte_RS_por_kWh_ponta 1.61 // Valor estimado pelo que o diretor de infraestrutura nos informou.
#define cte_RS_por_kVArh_ponta 1.61 // Valor estimado pelo que o diretor de infraestrutura nos informou.
#define cte_medicao_kVAR_por_pulso 0.144 // Valor certificado para o medidor do Campus Cedro.
#define cte_RS_por_kWh_fora_ponta 0.41 // Valor estimado pelo que o diretor de infraestrutura nos informou.
#define cte_RS_por_kVArh_fora_ponta 0.41 // Valor estimado pelo que o diretor de infraestrutura nos informou.

#define end_reposicao 0 // Endereço da posição inicial memória onde os valores guardados serão escritos.

byte octetos[8]={0,0,0,0,0,0,0,0}; //  Vetor de Bytes que acondiciona os dados recebidos da SSU.

byte alerta_consumo=1; // Para os casos de alerta de consumo 1: verde; 2: amarelo e 3: vermelho.

unsigned int conta_fim_intervalo=0; // Conta os zeros segundos restantes no fim do intervalo.
unsigned int conta_totalizacao=0; // Conta as reposições de demanda.
unsigned int tempo_restante_anterior=0;  // Tempo restante do intervalo de medição gravado no último conjunto de dados recebidos.
unsigned int segundos_intervalo=0; // Tempo total contado dentro do horário atual: fora ponta e ponta.
unsigned int tempo=0;  // tempo contato entre dados recebidos da SSU a fim de verificar o timeout. ACREDITO NÃO PRECISAR SER LONG. ANALISAR FUTURAMENTE.
unsigned int indice_octeto=0; // Índice que aponta para próxima região livre do vetor de octetos.
unsigned int ativos_horario_atual=0; // Valor do consumo ativo total no horário atual considerado: ponta ou fora ponta.
unsigned int ativos_ponta_total=0;  // Valor do consumo ativo totalizado no mês no horário de ponta.
unsigned int ativos_fora_ponta_total=0;  // Valor do consumo ativo totalizado no mês no horário fora ponta.
unsigned int reativos_ponta_total=0;  // Valor do consumo de reativo totalizado no mês no horário de ponta.
unsigned int reativos_horario_atual=0; // Valor do consumo reativo total no horário atual considerado: ponta ou fora ponta.
unsigned int reativos_fora_ponta_total=0;  // Valor do consumo reativo totalizado no mês no horário fora ponta.
unsigned int delta_ativos=0;  // Variação de ativos entre segundos consecutivos.
unsigned int delta_reativos=0;  // Variação de reativos entre segundos consecutivos.
unsigned int segundos_horario=0; // Tempo em segundos desde o início do horário atual (ponta ou fora ponta).

bool verificacao_ok=false;  // Flag que indica verificação de dados vindos da SSU está ok.
bool ocioso=true;   // Flag que sinaliza a não recepção de dados da SSU.
bool totalizacao=false; // Flag que sinaliza uma reposição de demanda.
bool fim_intervalo=false; // Flag para sinalisar fim do intervalo de 15 min de medição atual.
bool tarifa_mudou=false; // Flag para sinalisar fim do intervalo ponta ou fora ponta.

float valor_conta=0;  // Valor atualizado da conta em reais soma do valor_total_anterior e valor_intervalo.

// Variáveis decodificadas a partir dos dados recebidos pela SSU.
unsigned int tempo_restante=0; // Tempo restante em segundos para fim do intervalo de reposição de demanda. São 15 minutos (900 segundos) para cada intervalo.
unsigned short tarifa=0; //Ponta(1) ou Fora ponta(2), reservado (8);
unsigned short tarifa_anterior=0; //Ponta(1) ou Fora ponta(2), reservado (8);
unsigned short tipo=0; // Azul (0), Verde (1), irrigantes (2) e outras (3);
bool reposicao = false; // Complementado a cada reposição de demanda.
bool reposicao_anterior=false; // Grava o estado da reposição de demanda anterior.
bool int_reativo = false; // Indicador de intervalo reativo.
bool considera_capacitivo = false; // Indica se o reativo capacitivo é considerado no cálculo da conta.
bool considera_indutivo = false; // Indica se o reativo indutivo é considerado no cálculo da conta.
bool tarifa_reativo=0; // Ativada (1) ou desativada (0).

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

// Função que envia dados de consumo pela serial.
void Envia_dados_serial(){

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
