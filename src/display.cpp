#include "display.h"
#include <TFT_eSPI.h>
#include "ranking.h"  // Incluindo a referência ao ranking.cpp

TFT_eSPI tft = TFT_eSPI();  // Instância global para evitar reinicializações

void inicializarDisplay() {
    tft.init();
    tft.setRotation(1);
    tft.setTextSize(2);
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 40);
    tft.print("Press Start");
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

void exibirMensagem(String mensagem, int tamanho) {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 40);
    tft.setTextSize(tamanho); 
    tft.print(mensagem);
}

void exibirRanking() {
    String rankingTexto = lerRanking();  // Obtendo o ranking via função de ranking.cpp

    tft.fillScreen(TFT_BLACK);  
    tft.setCursor(20, 10);
    tft.setTextSize(1);         
    tft.setTextColor(TFT_WHITE);
    tft.println("RANKING");
    tft.print(rankingTexto);  // Exibindo o ranking no display
}
