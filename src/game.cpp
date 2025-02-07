// game.cpp

#include "game.h"
#include "pitches.h"
#include <TFT_eSPI.h>
#include "ranking.h"
#include "display.h"

const int totaljogadas = 100;

const uint8_t pinoled[] = {13, 2, 14, 27};
const uint8_t pinobotao[] = {26, 25, 33, 32};
bool jogoEmAndamento = false;  // Definição da variável jogoEmAndamento

// Definindo os tons para os LEDs
const int gameTones[] = { NOTE_G3, NOTE_C4, NOTE_E4, NOTE_G5 };  // Definição de gameTones

uint8_t sequencia[totaljogadas] = {0};
uint8_t gameIndex = 0;

void inicializarBotoesLeds(){
    for (byte i = 0; i < 4; i++) {
    pinMode(pinoled[i], OUTPUT);
    pinMode(pinobotao[i], INPUT_PULLUP);
  }
}

// Função para tocar a sequência de LEDs e sons
void ledetocar(byte ledIndex) {
    digitalWrite(pinoled[ledIndex], HIGH);
    tone(21, gameTones[ledIndex]);
    delay(300);
    digitalWrite(pinoled[ledIndex], LOW);
    noTone(21);
}

void tocarsequencia() {
    for (int i = 0; i < gameIndex; i++) {
        byte currentLed = sequencia[i];
        ledetocar(currentLed);
        delay(50);
    }
}

byte readButtons() {
    while (true) {
        for (byte i = 0; i < 4; i++) {
            byte cliquepinobotao = pinobotao[i];
            if (digitalRead(cliquepinobotao) == LOW) {
                delay(50);
                return i;
            }
        }
        delay(1);
    }
}

bool checarsequeciausuario() {
    for (int i = 0; i < gameIndex; i++) {
        byte esperadobotao = sequencia[i];
        byte atualbotao = readButtons();
        ledetocar(atualbotao);
        if (esperadobotao != atualbotao) {
            return false;
        }
    }
    return true;
}

void gameOver() {
    Serial.print("Game over! Seus pontos: ");
    Serial.println(gameIndex - 1);
    jogoEmAndamento = false;
    delay(200);

    // Exibe a mensagem de game over
    exibirMensagem("Errou :(", 2);

    // Toca os sons de game over
    tone(21, NOTE_DS5);
    delay(300);
    tone(21, NOTE_D5);
    delay(300);
    tone(21, NOTE_CS5);
    delay(300);

    // Toca um efeito sonoro
    for (byte i = 0; i < 10; i++) {
        for (int pitch = -10; pitch <= 10; pitch++) {
            tone(21, NOTE_C5 + pitch);
            delay(5);
        }
    }
    noTone(21);

    // Chama a função para atualizar o ranking
    String nomeJogador = "Jogador";  // Aqui, você pode modificar para pegar o nome do jogador real
    int pontos = gameIndex - 1;
    atualizarRanking(nomeJogador, pontos);  // Atualiza o ranking

    gameIndex = 0;  // Reseta a pontuação do jogo
    exibirRanking();  // Exibe o ranking atualizado
    delay(4000);

    Serial.println("Aperte Start para iniciar um novo jogo");
    exibirStart();
}


void musicanivel() {
    tone(21, NOTE_E4);
    delay(150);
    tone(21, NOTE_G4);
    delay(150);
    tone(21, NOTE_E5);
    delay(150);
    tone(21, NOTE_C5);
    delay(150);
    tone(21, NOTE_D5);
    delay(150);
    tone(21, NOTE_G5);
    delay(150);
    noTone(21);
}
