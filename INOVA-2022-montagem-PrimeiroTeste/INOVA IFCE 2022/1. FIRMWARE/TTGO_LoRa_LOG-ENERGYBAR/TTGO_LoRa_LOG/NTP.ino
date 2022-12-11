#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>

const char *ssid     = "Valter";
const char *password = "12345678";

unsigned long tempo_absoluto=0;     // Conta o tempo em segundos desde a última sincronização de relógio.

unsigned long dias=0;
unsigned long mes=0;
unsigned long anos=1970;

void imprime_tempo();
void atualiza_relogio();
void atualiza_data();
bool ano_bisexto(int ano);
  
#define NTP_OFFSET  -3*60*60                // Desvio de tempo em segundos defasado de GMT.
#define NTP_INTERVAL 5 * 1000               // Intervalo de confiança em milisundos para seleção do servidor. Valor de tempo que o servidor pode variar sem que seja considerado um falso relógio.
#define NTP_ADDRESS  "1.asia.pool.ntp.org"  // Servidor de NTP.

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

void inicializa_WIFI(){
  informacoes_inicializacao_OLED("Inicializando Wifi...");
  
  // Inicializa a conexão WiFi.
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 2000 );
    Serial.print ( "." );
  }
  Serial.println(" -> Wifi conectado!");
}

void inicializa_NTP(){

  informacoes_inicializacao_OLED("Inicializando relógio...");
  
// Atualizando hora via NTP.
  timeClient.begin();
  timeClient.update();
  tempo_absoluto=millis();            // Guarda o tempo em que foi feito a sincronização com o servidor NTP.
   
  dia_semana=(int)timeClient.getDay();
  hora=(int)timeClient.getHours();
  minuto=(int)timeClient.getMinutes();
  segundo=(int)timeClient.getSeconds();
  segundo_anterior=segundo;
  
  decodifica_data();
  
  // Fecha conexão com o servidor NTP.
  timeClient.end(); // Caso queiramos fechar a conexão.

  Serial.println("Data e hora ok!");
  Serial.println((String)hora +":"+ minuto +":"+ segundo+"|"+ dias +"/"+ mes +"/"+ anos);
  
}

bool ano_bisexto(int ano){
  if((ano%4 == 0 && ano%100 != 0) || ano%400 == 0)
    return true;
  else
    return false;
}

void decodifica_data(){
  unsigned long tempoBruto = timeClient.getEpochTime()/86400;
  static const uint8_t dias_mes[]={31,28,31,30,31,30,31,31,30,31,30,31};

  while((dias += (ano_bisexto(anos) ? 366 : 365)) <= tempoBruto)
      anos++;
      
  tempoBruto -= dias - (ano_bisexto(anos) ? 366 : 365); // Agora o tempo bruto são dias deste ano iniciando em zero.
    
  dias=tempoBruto;
  
  for (mes=0; mes<12; mes++) {
    uint8_t comp_mes;
    
    if (mes==1) // fevereiro.
      comp_mes = ano_bisexto(anos) ? 29 : 28;
    else 
      comp_mes = dias_mes[mes];
    
    if (dias < comp_mes) 
      break;
    
    dias -= comp_mes;
  }
  // Tanto o mês quanto os dias são contados a partir do zero. Devemos corrigir acrescendo o dia 1 e o mês 1.
  dias++;
  mes++;
}

// Converte um número de 1 a 7 em dia da semana - string.
String converte_dia(int dia_semana){

  switch(dia_semana){
    case 1:   return "Segunda";
    case 2:   return "Terça";
    case 3:   return "Quarta";
    case 4:   return "Quinta";
    case 5:   return "Sexta";
    case 6:   return "Sábado";
    case 7:   return "Domingo";
    default:  return "erro";
  }
}

// Imprime os dados NTP pela serial.
void imprime_tempo(){
  Serial.print(dia_semana);          // Dia da semana.
  Serial.print(" - ");
  Serial.print(hora);        // Hora do dia.
  Serial.print(":");
  Serial.print(minuto);      // Minutos da hora.
  Serial.print(":");
  Serial.print(segundo);      // Segundos do minuto.
  Serial.print(" - ");
  Serial.print(dias);        // Hora do dia.
  Serial.print("/");
  Serial.print(mes);      // Minutos da hora.
  Serial.print("/");
  Serial.println(anos);      // Segundos do minuto.
}

void incrementa_data(){

  if(segundo>59){
      minuto++;
      segundo=0;
      if(minuto>59){
        hora++;
        minuto=0;
        if(hora>23){
          hora=0;
          // Atualiza dias.
          dias++;
          dia_semana=(++dia_semana)%7;       
      }
    }
  }
  
  // Atualiza mês.
  switch(mes)
  {
    case 1:
    case 3: 
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
      // Meses de 31 dias.
      if(dias>31)
      {
        dias=0;
        mes++;
      }
    break;
    case 2: 
      // Mês de fevereiro.
      if(ano_bisexto(anos))
      {
        if(dias>29)
        {
          dias=0;
          mes++; 
        }
      }
      else
      {
        if(dias>28)
        {
          dias=0;
          mes++; 
        }
      }
    break;
    case 4: 
    case 6:
    case 9:
    case 11:
      // Meses de 30 dias.
      if(dias>30)
      {
        dias=0;
        mes++;
      }
    break;
  }
  
  // Atualiza anos.
  if(mes>12)
  {
    mes=0;
    anos++;
  }
}
