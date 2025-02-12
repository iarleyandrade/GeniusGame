#include "telegram.h"
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "ranking.h"
#include "display.h"

#define PINO_SALVAR 25
#define PINO_NAO_SALVAR 26

const char* BOT_TOKEN = "7849953317:AAG9S_6T71XdPX-am1BkZ490_uJWGq5ZcCg";
const long INTERVALO_VERIFICACAO = 2000;
const long NUMERO_DESTINO = 1844187065;

bool esperaNome = false;
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

void inicializarTelegram() {
    client.setInsecure();
    Serial.println("Bot do Telegram inicializado.");
}

void interagirFimJogo(int pontuacao) {
    bot.sendMessage(String(NUMERO_DESTINO), "Deseja salvar seu placar? ('sim' aperte verde ou 'não' aperte vermelho)", "");
    while (true) {
        int leituraSalvar = digitalRead(PINO_SALVAR);
        int leituraNaoSalvar = digitalRead(PINO_NAO_SALVAR);

        if (leituraSalvar == LOW) {
            digitalWrite(22, HIGH);
            delay(100);
            digitalWrite(22, LOW);
            esperaNome = true;
            esperarNome(pontuacao);
            break;  
        } else if (leituraNaoSalvar == LOW) {
            digitalWrite(5, HIGH);
            delay(100);
            digitalWrite(5, LOW);
            bot.sendMessage(String(NUMERO_DESTINO), "Você escolheu não salvar seu placar.", "");
            esperaNome = false;
            break;
        }

        delay(100);
    }
}

void salvarPlacar(String texto, int pontuacao) {
    if (esperaNome) { 
        atualizarRanking(texto, pontuacao);
        bot.sendMessage(String(NUMERO_DESTINO), "Seu placar foi salvo com sucesso!", "");
        esperaNome = false;
    }
}

void mostrarRanking(String chat_id, String texto) {
    if (texto == "/ranking") {
        String rankingAtual = lerRanking();
        bot.sendMessage(chat_id, "Ranking atual:\n" + rankingAtual, "");
    }
}

void esperarNome(int pontuacao) {
    exibirMensagem("Digite seu \n nome no \n Telegram", 2, 10, 10);
    bot.sendMessage(String(NUMERO_DESTINO), "Digite seu nome para salvar no ranking:", "");

    unsigned long tempoInicial = millis(); 

    while (esperaNome) {  
        
        int numNovasMensagens = bot.getUpdates(bot.last_message_received + 1);

        if (numNovasMensagens > 0) {
            for (int i = 0; i < numNovasMensagens; i++) {
                String chat_id = bot.messages[i].chat_id;
                String textoRecebido = bot.messages[i].text;

                // Se a mensagem é do usuário correto
                if (chat_id.toInt() == NUMERO_DESTINO) {
                    salvarPlacar(textoRecebido, pontuacao);
                    exibirMensagem("Seu placar \n foi salvo!!", 2, 10, 10); 
                    esperaNome = false;
                    break;
                }
            }
        }

        if (millis() - tempoInicial > 30000) {
            bot.sendMessage(String(NUMERO_DESTINO), "Tempo esgotado! Seu placar não foi salvo.");
            exibirMensagem("Tempo \n esgotado!", 2, 10, 10); 
            esperaNome = false;
        }

        delay(100);
    }
}




