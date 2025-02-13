#ifndef TELEGRAM_H
#define TELEGRAM_H

#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

void salvarPlacar(String texto, int pontuacao);
void mostrarRanking(String chat_id, String texto);
void interagirFimJogo(int pontos);
void inicializarTelegram();
void esperarNome(int pontuacao);
void enviarRankingTelegram();

#endif
