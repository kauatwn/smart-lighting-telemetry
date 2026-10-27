/**
 * @file thing_properties.h
 * @brief Declara as propriedades da Arduino IoT Cloud.
 *
 * Expõe os controles de iluminação, as métricas operacionais e
 * os callbacks dos comandos remotos.
 */

#pragma once

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

// Controles remotos dos pontos de iluminação.
extern CloudSwitch cozinha;
extern CloudSwitch sala;
extern CloudSwitch banheiro;
extern CloudSwitch varanda;

// Métricas da cozinha.
extern float cozinha_on_time_s;
extern int cozinha_activations;
extern float cozinha_consumption_mwh;

// Métricas da sala.
extern float sala_on_time_s;
extern int sala_activations;
extern float sala_consumption_mwh;

// Métricas do banheiro.
extern float banheiro_on_time_s;
extern int banheiro_activations;
extern float banheiro_consumption_mwh;

// Métricas da varanda.
extern float varanda_on_time_s;
extern int varanda_activations;
extern float varanda_consumption_mwh;

// Intensidade do sinal Wi-Fi em dBm.
extern int wifi_rssi_dbm;

/**
 * @brief Registra as propriedades e configura o identificador do Thing.
 */
void init_cloud_properties();

/**
 * @brief Sincroniza o estado remoto da cozinha com o hardware.
 */
void on_cozinha_change();

/**
 * @brief Sincroniza o estado remoto da sala com o hardware.
 */
void on_sala_change();

/**
 * @brief Sincroniza o estado remoto do banheiro com o hardware.
 */
void on_banheiro_change();

/**
 * @brief Sincroniza o estado remoto da varanda com o hardware.
 */
void on_varanda_change();

// Gerenciador da conexão Wi-Fi utilizada pela Arduino IoT Cloud.
extern WiFiConnectionHandler ArduinoIoTPreferredConnection;
