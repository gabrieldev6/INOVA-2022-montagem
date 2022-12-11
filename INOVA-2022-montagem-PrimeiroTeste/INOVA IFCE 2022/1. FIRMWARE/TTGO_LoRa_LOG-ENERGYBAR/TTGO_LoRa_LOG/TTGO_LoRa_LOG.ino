#include <SPI.h>
#include <Wire.h>

// Protótipos de funções ENERGBAR.
void Decodifica();
void Envia_dados_serial();
void Atualiza_valor();

// Protótipos de funções SENSORES.
void atualiza_LDR();
bool inicializa_DHT11();
bool atualiza_temperatura_umidade();
void inicializa_UV();
void atualiza_UV();

// Protótipos de funções LoRa.
void inicializa_LORA();
void setLoRa_SPI();
bool recepcao_LORA();
void informacoes_lora_SERIAL();

// Protótipos de funções NTP.
void inicializa_WIFI();
void inicializa_NTP();
void imprime_tempo();
void decodifica_data();
bool ano_bisexto(int ano);
void informacoes_hora_data_OLED(int y, int x);
void incrementa_data();

// Protótipos de funções OLED.
void inicializa_OLED();
void informacoes_hora_data_OLED(int coluna, int linha);
void informacoes_inicializacao_OLED(String informacao);
void informacoes_pacote_recebido_LORA(int coluna, int linha);
void informacoes_contagem_pacote_recebido_LORA(int coluna, int linha);
void informacoes_temperatura_umidade(int coluna, int linha);
void informacoes_LDR(int coluna, int linha);
void informacoes_UV(int coluna, int linha);

// Protótipos de funções cartão SD.
void inicializa_SD();
void updateFileName();
void printHeader();
byte logDataSD();
void setSD_SPI();

// Variáveis para sensores.
float luz_visivel=0.0;
float luz_ir=0.0;
float indice_uv=0.0;

// Variáveis para SD.
char logFileName[13]; // Char string para armazenar o nome do arquivo de log

// Protótipos de funções cartão TIMER.
void inicializa_TIMER();
void onTimer();

// Variáveis para computo das médias em um minuto.
int        soma_ativos=0;
int        soma_reativos=0;
float      soma_temperatura=0;
float      soma_umidade=0;
int        soma_LDR=0;
float      soma_luz_visivel=0;
float      soma_luz_ir=0;
float      soma_indice_uv=0;
int        quantidade=0;

bool atualiza_data_flag=false;
bool gravado=false;

unsigned int ativos_anterior=0; // Valor do consumo ativo até o segundo anterior dentro do intervalo de 15 min atual.
unsigned int reativos_anterior=0; // Valor do consumo reativo até o segundo anterior dentro do intervalo de 15 min atual.
unsigned int ativos=0; // Pulsos que totalizam ativos. Devemos multiplicar por 144 para obter o valor em kwh.
unsigned int reativos=0;// Pulsos que totalizam reativos. Devemos multiplicar por 144 para obter o valor em kVArh.

int dia_semana=0;
int hora=0;
int minuto=0;
int segundo=0;
int segundo_anterior=0;

int temperatura=0;
int umidade=0;

int LDR=0;            // Valor medido pelo LDR em pull-up (reisitor de 1k).

void setup() {
  // Inicializa serial.
  Serial.begin(115200);
  while (!Serial);
  
  inicializa_OLED();
  inicializa_WIFI();
  inicializa_NTP();
    
  inicializa_SD();
  inicializa_LORA();
  inicializa_TIMER();
    
  inicializa_DHT11();
  delay(1000);

  inicializa_UV();
  delay(1000);
}

void loop() {
   // Verifica se chegou novo pacote lora.
   if(recepcao_LORA())
   {
        // Decodifica dados recebisos pela SSU-LoRa.
        Decodifica();
        
        // Atualiza o valor da conta.
        Atualiza_valor();

        soma_ativos+=ativos-ativos_anterior;
        soma_reativos+=reativos-reativos_anterior;
   }
   
   // Atualiza a tela OLED.
   atualiza_TELA_OLED();

      // A cada segundo.
      if(segundo_anterior!=segundo){
        
        segundo_anterior=segundo;
        
        // Flag de gravação no arquivo.
        gravado=false;
        
        // Atualiza temperatura e umidade.
        atualiza_temperatura_umidade();

        // Atualiza LDR.
        atualiza_LDR();

        // Atualiza UV.
        atualiza_UV();

        quantidade++;

        soma_temperatura+=temperatura;
        soma_umidade+=umidade;
        soma_LDR+=LDR;
        soma_luz_visivel+=luz_visivel;
        soma_luz_ir+=luz_ir;
        soma_indice_uv+=indice_uv;

      }

      // A cada minuto fechado, grava-se os valores médios das variáveis acumuladas.
      if(segundo==0 && !gravado){    
        gravado=true;      
        // Guarda os dados no cartão SD.
        // Mudamos o barramento SPI para comunicar com o cartão SD.
        setSD_SPI();
        
        logDataSD();
        
        // Mudamos o barramento SPI para comunicar com o rádio LoRa.
        setLoRa_SPI();
        
        // Zera as variáveis para a próxima média.
        quantidade=0;
        
        soma_ativos=0;
        soma_reativos=0;
        soma_temperatura=0;
        soma_umidade=0;
        soma_LDR=0;
        soma_luz_visivel=0;
        soma_luz_ir=0;
        soma_indice_uv=0;
      }

      if(atualiza_data_flag){
        // Baixa a flag.
        atualiza_data_flag=false;
        incrementa_data();
      }
}
