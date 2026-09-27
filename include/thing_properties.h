/*
 * Cloud Properties Interface - Smart Lighting Telemetry
 *
 * Descrição:
 * Declaração das variáveis sincronizadas com o Arduino Cloud, protótipos de
 * callbacks de atuação remota e inicializador da conexão de rede.
 */

#pragma once

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

#include "secrets.h"

// Variáveis de controle sincronizadas com a nuvem (Atuação)
extern bool light1;
extern bool light2;
extern bool light3;
extern bool light4;
extern int brightness_pct;
extern int operation_mode;

// Variáveis de telemetria da placa (Leitura)
extern float temperature_c;
extern int wifi_rssi;
extern int uptime_minutes;
extern String status_message;

// Protótipos de callbacks de eventos disparados pelo Arduino Cloud
void on_light1_change();
void on_light2_change();
void on_light3_change();
void on_light4_change();
void on_brightness_change();
void on_operation_mode_change();

// Registra as propriedades e inicializa os identificadores do dispositivo
void init_cloud_properties();

extern WiFiConnectionHandler ArduinoIoTPreferredConnection;
