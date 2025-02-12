#include "telegram.h"
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "ranking.h"

#define PINO_SALVAR 25
#define PINO_NAO_SALVAR 26

const char* BOT_TOKEN = "7849953317:AAG9S_6T71XdPX-am1BkZ490_uJWGq5ZcCg";
const long INTERVALO_VERIFICACAO = 2000;
const long NUMERO_DESTINO = 1844187065;

bool esperaNome = false;
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

void inicializarTelegram() {
    client.setInsecure();  // Desativa verificação SSL (necessário para ESP32)
    Serial.println("Bot do Telegram inicializado.");
}

void interagirFimJogo(int pontuacao) {
    while (true) {
        int leituraSalvar = digitalRead(PINO_SALVAR);
        int leituraNaoSalvar = digitalRead(PINO_NAO_SALVAR);

        if (leituraSalvar == LOW) {
            digitalWrite(22, HIGH);
            delay(100);  // Delay para evitar múltiplos acionamentos rápidos
            digitalWrite(22, LOW);
            bot.sendMessage(String(NUMERO_DESTINO), "Deseja salvar seu placar? (responda 'sim' ou 'não')", "");
            esperaNome = true;  // Marca que está esperando o nome
            esperarNome(pontuacao);  // Chama a função para esperar o nome do jogador
            break;  
        } else if (leituraNaoSalvar == LOW) {
            digitalWrite(5, HIGH);
            delay(100);  // Delay para evitar múltiplos acionamentos rápidos
            digitalWrite(5, LOW);
            bot.sendMessage(String(NUMERO_DESTINO), "Você escolheu não salvar seu placar.", "");
            esperaNome = false;  // Não vai salvar o placar
            break;  // Sai do loop
        }

        delay(100);  // Delay para evitar looping rápido demais
    }
}

void salvarPlacar(String texto, int pontuacao) {
    if (esperaNome) { 
        atualizarRanking(texto, pontuacao);  // Atualiza o ranking com o nome e pontuação
        bot.sendMessage(String(NUMERO_DESTINO), "Seu placar foi salvo com sucesso!", "");
        esperaNome = false;  // Reseta o estado de espera
    }
}

void mostrarRanking(String chat_id, String texto) {
    if (texto == "/ranking") {
        String rankingAtual = lerRanking();
        bot.sendMessage(chat_id, "Ranking atual:\n" + rankingAtual, "");
    }
}

void esperarNome(int pontuacao) {
    // Espera o nome do jogador
    int numNovasMensagens = bot.getUpdates(bot.last_message_received + 1);

    while (numNovasMensagens > 0 && esperaNome) {
        for (int i = 0; i < numNovasMensagens; i++) {
            String chat_id = bot.messages[i].chat_id;
            String textoRecebido = bot.messages[i].text;

            // Verifica se a mensagem é do chat correto (do jogador que está jogando)
            if (esperaNome && chat_id.toInt() == NUMERO_DESTINO) {
                salvarPlacar(textoRecebido, pontuacao);  // Chama para salvar o placar com o nome
                esperaNome = false;  // Reseta o estado de espera
                break;  // Sai do loop após processar o nome
            }
        }

        numNovasMensagens = bot.getUpdates(bot.last_message_received + 1);  // Atualiza novas mensagens
        delay(100);  // Delay para evitar looping rápido demais
    }
}
