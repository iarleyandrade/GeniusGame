//GENIUS

#include "pitches.h"
#include <cstdint>
#include <Arduino.h>
#include <TFT_eSPI.h>
  
 
const uint8_t pinoled[] = {13, 2, 14, 27};
const uint8_t pinobotao[] = {26, 25, 33, 32};
#define speaker 21
#define botao_start 19
#define totaljogadas 100
 
 
const int gameTones[] = { NOTE_G3, NOTE_C4, NOTE_E4, NOTE_G5};
 
uint8_t sequencia[totaljogadas] = {0};
uint8_t gameIndex = 0;
bool jogoEmAndamento = false; 
TFT_eSPI tft = TFT_eSPI();
 

void setup() {
  Serial.begin(115200);  
  for (byte i = 0; i < 4; i++) {
    pinMode(pinoled[i], OUTPUT);
    pinMode(pinobotao[i], INPUT_PULLUP);
  }
  tft.init();                 // Inicializa o display
  tft.setRotation(1);         // Ajusta a rotação
  tft.setTextSize(2);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(10, 40);
  tft.print("Press Start");
  pinMode(speaker, OUTPUT);
  pinMode(botao_start, INPUT_PULLUP);
  Serial.println("Aperte o botao de start para iniciar o jogo!");
}
 
 
/**
   Exibindo o LED e tocando o tom do mesmo
*/
void ledetocar(byte ledIndex) {
  digitalWrite(pinoled[ledIndex], HIGH);
  tone(speaker, gameTones[ledIndex]);
  delay(300);
  digitalWrite(pinoled[ledIndex], LOW);
  noTone(speaker);
}
 
/**
   Tocando a Sequencia autal com notas para o Usuário Repetir
*/
void tocarsequencia() {
  for (int i = 0; i < gameIndex; i++) {
    byte currentLed = sequencia[i];
    ledetocar(currentLed);
    delay(50);
  }
}
 
/**
    Esperando pressionar um do botões,
    e retornar o indice de botões
*/
byte readButtons() {
  while (true) {
    for (byte i = 0; i < 4; i++) {
      byte cliquepinobotao = pinobotao[i];
      if (digitalRead(cliquepinobotao) == LOW) {
        return i;
      }
    }
    delay(1);
  }
}

 
/**
  Tocando game over sequencia, e mostrando pontos
*/
void gameOver() {
  Serial.print("Game over! Seus pontos: ");
  Serial.println(gameIndex - 1);
  gameIndex = 0;
  jogoEmAndamento = false;
  delay(200);
  // Sequência de Game Over
  tft.fillScreen(TFT_BLACK);
  delay(1000);
  tft.setCursor(10, 40);
  tft.print("Errou :(");

  tone(speaker, NOTE_DS5);
  delay(300);
  tone(speaker, NOTE_D5);
  delay(300);
  tone(speaker, NOTE_CS5);
  delay(300);
  for (byte i = 0; i < 10; i++) {
    for (int pitch = -10; pitch <= 10; pitch++) {
      tone(speaker, NOTE_C5 + pitch);
      delay(5);
    }
  }
  noTone(speaker);
  delay(600);
  Serial.println("Aperte Start para iniciar um novo jogo");
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(10, 40);
  tft.print("Press Start");
}
 
/**
   Pegando o botão precionado e compado com a sequencia do computador.
*/
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
 
/**
   Musica de Subida de Nível
*/
void musicanivel() {
  tone(speaker, NOTE_E4);
  delay(150);
  tone(speaker, NOTE_G4);
  delay(150);
  tone(speaker, NOTE_E5);
  delay(150);
  tone(speaker, NOTE_C5);
  delay(150);
  tone(speaker, NOTE_D5);
  delay(150);
  tone(speaker, NOTE_G5);
  delay(150);
  noTone(speaker);
}
 
 
/**
   Loop principal
*/
void loop() {
  if (digitalRead(botao_start) == LOW || jogoEmAndamento) {
    if (!jogoEmAndamento)
    {
      Serial.println("Jogo iniciado!");
    }else
    {
      Serial.println("Nova Rodada Iniciada");
    }
    jogoEmAndamento = true;
    delay(300); // Para evitar múltiplos registros do clique
    
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 40);
    tft.print("Pontos: ");
    tft.print(gameIndex);
 
  for (int i = 0 ; i<random(2, 15); i++){
    sequencia[gameIndex] = random(0, 4); //aleatório inicio 0 até menor 4 q é 3
  }
  gameIndex++; //aumenta o indice
  if (gameIndex >= totaljogadas) {
    gameIndex = totaljogadas - 1;
  }
 
  //exibe a sequencia
  tocarsequencia();
 
  //checa sequencia
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
}