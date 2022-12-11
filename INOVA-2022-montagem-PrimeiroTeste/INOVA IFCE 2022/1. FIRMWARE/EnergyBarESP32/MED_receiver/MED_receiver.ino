#include <WiFi.h>
#include <LoRa.h>
#include <SSD1306.h>

//#include <math.h>
//#include <Wire.h>
//#include <SPI.h>
//#include <Arduino.h>

SSD1306  display(0x3c, 4, 15);
 
//OLED pins to ESP32 GPIOs via this connection:
//OLED_SDA -- GPIO4
//OLED_SCL -- GPIO15
//OLED_RST -- GPIO16
// WIFI_LoRa_32 ports
// GPIO5  -- SX1278's SCK
// GPIO19 -- SX1278's MISO
// GPIO27 -- SX1278's MOSI
// GPIO18 -- SX1278's CS
// GPIO14 -- SX1278's RESET
// GPIO26 -- SX1278's IRQ(Interrupt Request)
 
#define SS      18
#define RST     14
#define DI0     26
#define BAND    433E6

#define AP false  // Caso seja true, o esp32 funciona em modo acess point, em false, funciona como cliente wifi.

const char *ssid_ap = "ESP32ap";
const char *password_ap = "12345678";

const char *ssid_client = "acesse ja lig 985191533 v"; //"Valter";
const char *password_client = "sou1pessoaFELIZ";  //"12345678";

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):


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

String data_="";   // Recebe os dados do pacote LoRa.

byte alerta_consumo=1; // Para os casos de alerta de consumo 1: verde; 2: amarelo e 3: vermelho.

unsigned int conta_fim_intervalo=0; // Conta os zeros segundos restantes no fim do intervalo.
unsigned int conta_totalizacao=0; // Conta as reposições de demanda.
unsigned int tempo_restante_anterior=0;  // Tempo restante do intervalo de medição gravado no último conjunto de dados recebidos.
unsigned int segundos_intervalo=0; // Tempo total contado dentro do horário atual: fora ponta e ponta.
unsigned int tempo=0;  // tempo contato entre dados recebidos da SSU a fim de verificar o timeout. ACREDITO NÃO PRECISAR SER LONG. ANALISAR FUTURAMENTE.
unsigned int indice_octeto=0; // Índice que aponta para próxima região livre do vetor de octetos.
unsigned int ativos_anterior=0; // Valor do consumo ativo até o segundo anterior dentro do intervalo de 15 min atual.
unsigned int ativos_horario_atual=0; // Valor do consumo ativo total no horário atual considerado: ponta ou fora ponta.
unsigned int ativos_ponta_total=0;  // Valor do consumo ativo totalizado no mês no horário de ponta.
unsigned int ativos_fora_ponta_total=0;  // Valor do consumo ativo totalizado no mês no horário fora ponta.
unsigned int reativos_anterior=0; // Valor do consumo reativo até o segundo anterior dentro do intervalo de 15 min atual.
unsigned int reativos_ponta_total=0;  // Valor do consumo de reativo totalizado no mês no horário de ponta.
unsigned int reativos_horario_atual=0; // Valor do consumo reativo total no horário atual considerado: ponta ou fora ponta.
unsigned int reativos_fora_ponta_total=0;  // Valor do consumo reativo totalizado no mês no horário fora ponta.
unsigned int delta_ativos=0;  // Variação de ativos entre segundos consecutivos.
unsigned int delta_reativos=0;  // Variação de reativos entre segundos consecutivos.
unsigned int segundos_horario=0; // Tempo em segundos desde o início do horário atual (ponta ou fora ponta).

byte octetos[8]=""; //  Vetor de Bytes que acondiciona os dados recebidos da SSU.
byte oux_byte=0;  // Ou exclusivo de todos os dados recebidos pela SSU até o momento. Ao final, deve-se realizar a inversão desta variável para a comparação com o Byte de verificação.

bool verificacao_ok=false;  // Flag que indica verificação de dados vindos da SSU está ok.
bool ocioso=true;   // Flag que sinaliza a não recepção de dados da SSU.
bool totalizacao=false; // Flag que sinaliza uma reposição de demanda.
bool fim_intervalo=false; // Flag para sinalisar fim do intervalo de 15 min de medição atual.
bool tarifa_mudou=false; // Flag para sinalisar fim do intervalo ponta ou fora ponta.

float valor_conta=0;  // Valor atualizado da conta em reais soma do valor_total_anterior e valor_intervalo.

// Variáveis decodificadas a partir dos dados recebidos pela SSU.
unsigned int tempo_restante=0; // Tempo restante em segundos para fim do intervalo de reposição de demanda. São 15 minutos (900 segundos) para cada intervalo.
unsigned int ativos=0; // Pulsos que totalizam ativos. Devemos multiplicar por 144 para obter o valor em kwh.
unsigned int reativos=0;// Pulsos que totalizam reativos. Devemos multiplicar por 144 para obter o valor em kVArh.
unsigned short tarifa=0; //Ponta(1) ou Fora ponta(2), reservado (8);
unsigned short tarifa_anterior=0; //Ponta(1) ou Fora ponta(2), reservado (8);
unsigned short tipo=0; // Azul (0), Verde (1), irrigantes (2) e outras (3);
bool reposicao = false; // Complementado a cada reposição de demanda.
bool reposicao_anterior=false; // Grava o estado da reposição de demanda anterior.
bool int_reativo = false; // Indicador de intervalo reativo.
bool considera_capacitivo = false; // Indica se o reativo capacitivo é considerado no cálculo da conta.
bool considera_indutivo = false; // Indica se o reativo indutivo é considerado no cálculo da conta.
bool tarifa_reativo=0; // Ativada (1) ou desativada (0).

// Set web server port number to 80
WiFiServer server(80);
WiFiClient client;
// Variable to store the HTTP request
String header;

//Protótipos de funções.
void Decodifica();
void Envia_dados_serial();
void Atualiza_valor();
void Alerta();

