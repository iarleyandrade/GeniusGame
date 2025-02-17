#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include "display.h"
#include <LittleFS.h>

extern bool conectadoWifi;

bool lerEstadoWifi() {
    if (!LittleFS.begin()) {
        Serial.println("Falha ao montar o sistema de arquivos.");
        return false;
    }

    if (LittleFS.exists("/wifi_status.txt")) {
        File file = LittleFS.open("/wifi_status.txt", "r");
        if (file) {
            String status = file.readString();
            file.close();
            status.trim();
            if (status == "true") {
                return true;
            }
        }
    }
    return false;
}


void salvarEstadoWifi(bool estado) {
    File file = LittleFS.open("/wifi_status.txt", "w");
    if (file) {
        file.println(estado ? "true" : "false");
        file.close();
    }
}

void iniciarWiFi() {
    Serial.begin(115200);
    delay(10);

    WiFiManager wifiManager;
    wifiManager.setConfigPortalTimeout(240);

    conectadoWifi = lerEstadoWifi();

    if (WiFi.SSID().isEmpty() && !conectadoWifi) {
        exibirMensagem("Deseja configurar o Wifi?\nVermelho - Nao\nVerde - Sim", 1, 0, 10); 

        while (true) {
            if (digitalRead(26) == LOW) {
                digitalWrite(5, HIGH);
                delay(300);
                digitalWrite(5, LOW);

                conectadoWifi = false;
                salvarEstadoWifi(false);
                Serial.println("Pulado Wi-Fi. Iniciando jogo offline...");
                exibirMensagem("Modo Offline", 2, 10, 10);
                delay(1000);
                return;
            }

            if (digitalRead(25) == LOW) {
                digitalWrite(22, HIGH);
                delay(300);
                digitalWrite(22, LOW);
                exibirMensagem("Configure o\n Wifi", 2, 10, 10);

                if (!wifiManager.autoConnect("Genius Game", "12345678")) {
                    Serial.println("Falha na conexão. Resetando e tentando novamente...");
                    delay(3000);
                    ESP.restart();
                }

                exibirMensagem("Iniciando\n Genius!", 2, 10, 10);
                conectadoWifi = true;
                salvarEstadoWifi(true);
                delay(1000);

                Serial.println("Conectado ao Wi-Fi.");
                Serial.print("IP: ");
                Serial.println(WiFi.localIP());

                return;
            }

            delay(200);
        }
    }

    
    if (!wifiManager.autoConnect("Genius Game", "12345678")) {
        Serial.println("Falha na conexão. Resetando e tentando novamente...");
        delay(3000);
        salvarEstadoWifi(false);
        ESP.restart();
    }

    if (conectadoWifi) {
        exibirMensagem("Iniciando\n Genius!", 2, 10, 10);
        Serial.println("Conectado ao Wi-Fi.");
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
        delay(1000);
    } else {
        exibirMensagem("Modo Offline", 2, 10, 10);
        Serial.println("Iniciando jogo offline...");
        delay(1000);
    }
}

