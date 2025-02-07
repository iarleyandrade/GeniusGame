// game.h

#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include <TFT_eSPI.h>

extern const uint8_t pinoled[];
extern const uint8_t pinobotao[];
extern const int gameTones[]; 

extern uint8_t sequencia[];
extern uint8_t gameIndex;
extern bool jogoEmAndamento;   
extern TFT_eSPI tft;

extern const int totaljogadas;

extern void inicializarBotoesLeds();
extern void ledetocar(byte ledIndex);
extern void tocarsequencia();
extern byte readButtons();
extern bool checarsequeciausuario();
extern void gameOver();
extern void musicanivel();

#endif
