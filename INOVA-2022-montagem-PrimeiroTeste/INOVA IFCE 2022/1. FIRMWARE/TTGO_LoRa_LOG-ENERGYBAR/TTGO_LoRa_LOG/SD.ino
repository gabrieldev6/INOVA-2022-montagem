#include <SD.h>

// Lembre-se de que a biblioteca SD tem comprimentos máximos de nome de arquivo de 8,3 a 8 caracteres e um sufixo de 3 caracteres.
// Nossos arquivos de log são chamados de "gpslogXX.csv, então" gpslog99.csv "é o nosso arquivo max.
#define LOG_FILE_PREFIX "ebar" // Nome do arquivo de log.
#define MAX_LOG_FILES 1000 // Número de arquivos de log que podem ser feitos
#define LOG_FILE_SUFFIX "data" // Sufixo do arquivo de log
// Dados a serem registrados:
#define LOG_COLUMN_COUNT 15
char * log_col_names[LOG_COLUMN_COUNT] = {
  "Dia", "Mês", "Anos", "Hora", "Minuto", "Dia Semana", "Ativos", "Reativos", "Temperatura", "Umidade", "LDR", "Luz Visível", "Luz IR", "Índice UV" 
}; // log_col_names é impresso na parte superior do arquivo de texto.

#define SCK_SD       14   // GPIO5  -- SX1278's SCK
#define MISO_SD      2    // GPIO19 -- SX1278's MISO
#define MOSI_SD      15   // GPIO27 -- SX1278's MOSI
#define SS_SD        13   // GPIO18 -- SX1278's CS

void inicializa_SD() {

  informacoes_inicializacao_OLED("Inicializando SDcard...");

  // Inicializando cartão SD.
  Serial.println("Initializing SD card...");

  setSD_SPI();

  if (!SD.begin()) {
    Serial.println("Cartão falhou ou não está plugado.");
    //Para a execução até que um cartão seja reconhecido.
    while (!SD.begin()) {
      delay(1000);
    }
  }
  Serial.println("Inicializou o cartão SD.");

  updateFileName();
  //printHeader();
}

void setSD_SPI() {
  // Para a comunicação SPI.
  SPI.end();
  //pinMode(MISO_SD,INPUT_PULLUP);
  // Inicializa os pinos para comunicação com o cartão SD da placa TTGO.
  SPI.begin(SCK_SD, MISO_SD, MOSI_SD, SS_SD);
}

byte logDataSD()
{
  File logFile = SD.open(logFileName, FILE_APPEND); // Abrir o log de arquivos

  if (logFile)
  { //"Dia", "Mês", "Anos", "Hora", "Minuto", "Dia Semana", "Ativos", "Reativos", "Temperatura", "Umidade", "LDR", "Luz Visível", "Luz IR", "Índice UV" 
    logFile.print(dias);
    //logFile.print(",");
    logFile.print("\t");
    logFile.print(mes);
    //logFile.print(",");
    logFile.print("\t");
    logFile.print(anos);
    //logFile.print(",");
    logFile.print("\t");
    logFile.print(hora);
    //logFile.print(",");
    logFile.print("\t");
    logFile.print(minuto);
    //logFile.print(",");
    logFile.print("\t");
    logFile.print(dia_semana);
    //logFile.print(",");
    logFile.print("\t");
    logFile.print(soma_ativos);
    //logFile.print(","); 
    logFile.print("\t");   
    logFile.print(soma_reativos);
    //logFile.print(",");
    logFile.print("\t");
    logFile.print((float)soma_temperatura/(float)quantidade);
    //logFile.print(",");
    logFile.print("\t");
    logFile.print((float)soma_umidade/(float)quantidade);
    //logFile.print(",");
    logFile.print("\t");
    logFile.print((float)soma_LDR/(float)quantidade);
    //logFile.print(",");
    logFile.print("\t");
    logFile.print(soma_luz_visivel/(float)quantidade);
    //logFile.print(",");
    logFile.print("\t");
    logFile.print(soma_luz_ir/(float)quantidade);
    //logFile.print(",");
    logFile.print("\t");
    logFile.println(soma_indice_uv/(float)quantidade);
        
    logFile.close();

    return 1; // Retorne sucesso
  }

  return 0; // Se falharmos em abrir o arquivo, retorne a falha
}

// Imprime nossos oito nomes de colunas no topo do nosso arquivo de logfile
void printHeader()
{
  File logFile = SD.open(logFileName, FILE_WRITE); // Abrir o arquivo de logo

  if (logFile) // Se o arquivo de log for aberto, imprima os nomes das colunas no arquivo
  {
    int i = 0;
    for (; i < LOG_COLUMN_COUNT; i++)
    {
      logFile.print(log_col_names[i]);
      if (i < LOG_COLUMN_COUNT - 1) // Se é tudo menos a última coluna
        logFile.print(','); // imprimir uma vírgula
      else // Se é a última coluna
        logFile.println(); // criar uma nova linha
    }
    logFile.close(); // Fecha o arquivo
  }
}

// Olha através dos arquivos de log já presentes em um cartão,
// e cria um novo arquivo com um índice de arquivo incrementado.
void updateFileName()
{
  int i = 0;
  for (; i < MAX_LOG_FILES; i++)
  {
    memset(logFileName, 0, strlen(logFileName)); // Limpar cadeia logFileName
    // Set logFileName to "/ebarXX.data":
    sprintf(logFileName, "/%s%d.%s", LOG_FILE_PREFIX, i, LOG_FILE_SUFFIX);
    if (!SD.exists(logFileName)) // Se o arquivo não existeir
    {
      break; // Para e sair do loop.
    }

  }
  Serial.print("File name: ");
  Serial.println(logFileName); // imprimir o nome do arquivo
}
