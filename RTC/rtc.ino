#include <Arduino.h>
#include <DS1307.h> //essa biblioteca esta disponivel dentro da pasta libraries

//cria um objeto onde as portas utilizadas seram d5 e d6
DS1307 rtc(D5, D6); //SDA, SCL


void setup() {
    //inicia o rtc
    rtc.halt(false);
    //seta o dia da semana, hora e a data de onde deve comecar a contar    
    rtc.setDOW(SUNDAY);
    rtc.setTime(20, 20, 20);
    rtc.setDate(5, 12, 2022);
    rtc.setSQWRate(SQW_RATE_1);
    rtc.enableSQW(true);

    Serial.begin(9600);
}

void loop() {
    
    Serial.print("Hora : ");
    Serial.print(rtc.getTimeStr());
    Serial.print(" ");
    Serial.print(rtc.getDateStr(FORMAT_SHORT));
    Serial.print(" ");
    Serial.println(rtc.getDOWStr(FORMAT_SHORT));
    //atualizacao e feita de forma automatica, mas ele mostra na tela de 2 em 2 segundo
    delay(2000);
}