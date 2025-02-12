#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include "display.h"

void iniciarWiFi() {
    Serial.begin(115200);
    delay(10);

    WiFiManager wifiManager;
    wifiManager.setConfigPortalTimeout(240);


    if (WiFi.SSID().isEmpty()) {
        exibirMensagem("Configure o\n Wifi", 2, 10, 10); 
    }

    if (!wifiManager.autoConnect("Genius Game", "12345678")) {
        Serial.println(F("Falha na conexao. Resetar e tentar novamente..."));
        delay(3000);
        ESP.restart();
        delay(5000);
    }


    exibirMensagem("Iniciando\n Genius!", 2, 10, 10); 
    delay(1000);

    Serial.println(F("Conectado na rede Wifi."));
    Serial.print(F("Endereco IP: "));
    Serial.println(WiFi.localIP());
}
