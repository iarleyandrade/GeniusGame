#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

void inicializarDisplay();
void exibirStart();
void exibirPlacar(int pontos);
void exibirMensagem(String mensagem, int tamanho);
void exibirRanking();

#endif
