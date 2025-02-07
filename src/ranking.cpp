#include "ranking.h"
#include "LittleFS.h"

void atualizarRanking(String nomeJogador, int pontuacao) {
    struct Jogador {
        String nome;
        int pontos;
    };

    Jogador ranking[5];  
    int count = 0;

    File file = LittleFS.open("/ranking.txt", "r");
    if (!file) {
        Serial.println("Erro ao abrir ranking.txt!");
        return;
    }

    while (file.available() && count < 5) {
        String linha = file.readStringUntil('\n');
        int separador = linha.indexOf(':');
        if (separador != -1) {
            ranking[count].nome = linha.substring(0, separador);
            ranking[count].pontos = linha.substring(separador + 1).toInt();
            count++;
        }
    }
    file.close();

    bool atualizado = false;
    for (int i = 0; i < 5; i++) {
        if (pontuacao > ranking[i].pontos) {
            for (int j = 4; j > i; j--) {
                ranking[j] = ranking[j - 1];
            }
            ranking[i].nome = nomeJogador;
            ranking[i].pontos = pontuacao;
            atualizado = true;
            break;
        }
    }

    if (atualizado) {
        file = LittleFS.open("/ranking.txt", "w");
        if (!file) {
            Serial.println("Erro ao abrir ranking.txt para escrita!");
            return;
        }

        for (int i = 0; i < 5; i++) {
            file.println(ranking[i].nome + ":" + String(ranking[i].pontos));
        }
        file.close();
        Serial.println("Ranking atualizado!");
    } else {
        Serial.println("Pontuação não entrou no ranking.");
    }
}

String lerRanking() {
    String rankingTexto = "";
    File file = LittleFS.open("/ranking.txt", "r");
    if (!file) {
        Serial.println("Erro ao abrir ranking.txt!");
        return "Erro ao abrir ranking!";
    }

    while (file.available()) {
        rankingTexto += file.readStringUntil('\n') + "\n";
    }

    file.close();
    return rankingTexto;
}
