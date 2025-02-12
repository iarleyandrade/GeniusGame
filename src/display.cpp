#include "display.h"
#include <TFT_eSPI.h>
#include "ranking.h"

TFT_eSPI tft = TFT_eSPI();

void inicializarDisplay() {
    tft.init();
    tft.setRotation(1);
    tft.setTextSize(2);
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 40);
}


void exibirStart() {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 40);
    tft.setTextSize(2);
    tft.print("Press Start");
}

void exibirPlacar(int pontos) {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 40);
    tft.setTextSize(2);  
    tft.print("Pontos: ");
    tft.print(pontos);
}

void exibirMensagem(String mensagem, int tamanho, int x, int y) {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(x, y);
    tft.setTextSize(tamanho); 
    tft.print(mensagem);
}

void exibirRanking() {
    String rankingTexto = lerRanking();

    tft.fillScreen(TFT_BLACK);  
    tft.setCursor(35, 10);
    tft.setTextSize(1);         
    tft.setTextColor(TFT_WHITE);
    tft.println("RANKING");
    tft.print(rankingTexto);
}
