#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <WiFi.h>
#include <WiFiManager.h>

void iniciarWiFi();
bool lerEstadoWifi();
void salvarEstadoWifi(bool estado);

#endif
