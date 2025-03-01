#include <Arduino.h>
#include "LittleFS.h"
#include <TFT_eSPI.h>
#include "ranking.h"
#include "game.h"
#include "display.h"
#include "wifi_manager.h"
#include "telegram.h"


#define botao_start 19
static unsigned long ultimoTempoTelegram = 0;
unsigned long intervaloTelegram = 5000;
extern bool conectadoWifi;

void setup() {
    Serial.begin(115200);

    if (!LittleFS.begin(true)) {
        Serial.println("Erro ao montar o LittleFS!");
        delay(3000);
        ESP.restart();
    }

    if (!LittleFS.exists("/ranking.txt")) {
        File file = LittleFS.open("/ranking.txt", "w");
        if (file) {
            file.println("N/A:0");
            file.println("N/A:0");
            file.println("N/A:0");
            file.println("N/A:0");
            file.println("N/A:0");
            file.close();
        }
    }

    pinMode(botao_start, INPUT_PULLUP);
    inicializarBotoesLeds();
    inicializarDisplay();

    iniciarWiFi();
    inicializarTelegram();
    
    exibirStart();
}

void loop() {
    if (digitalRead(botao_start) == LOW || jogoEmAndamento) {
        if (!jogoEmAndamento) {
            Serial.println("Jogo iniciado!");
        }
        jogoEmAndamento = true;
        delay(300);
        
        exibirPlacar(gameIndex);

        sequencia[gameIndex] = random(0, 4);
        gameIndex++;

        if (gameIndex >= totaljogadas) {
            gameIndex = totaljogadas - 1;
        }

        tocarsequencia();

        if (!checarsequeciausuario()) {
            gameOver();
        }

        delay(300);

        if (gameIndex > 0) {
            Serial.print("Pontos: ");
            Serial.println(gameIndex);
            musicanivel();
            delay(300);
        }
    }

    if (!jogoEmAndamento && conectadoWifi && millis() - ultimoTempoTelegram > intervaloTelegram) {
        ultimoTempoTelegram = millis();
        enviarRankingTelegram();
    }
    

}
