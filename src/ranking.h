#ifndef RANKING_H
#define RANKING_H

#include <Arduino.h>

void atualizarRanking(String nomeJogador, int pontuacao);

String lerRanking();
bool verificarPontuacaoRanking(int pontuacao);

#endif
