/**
 * @file thing_properties.h
 * @brief Declaração das propriedades e conexões da Arduino IoT Cloud.
 *
 * Expõe as variáveis sincronizadas com a nuvem e os comandos remotos
 * integrados com a Amazon Alexa.
 */

#pragma once

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

// Controles remotos dos pontos de iluminação (Lâmpadas na Alexa)
extern CloudLight cozinha;
extern CloudLight sala;
extern CloudLight banheiro;
extern CloudLight varanda;

// Métricas de telemetria da Cozinha
extern float cozinha_on_time_s;
extern int cozinha_activations;
extern float cozinha_consumption_mwh;

// Métricas de telemetria da Sala
extern float sala_on_time_s;
extern int sala_activations;
extern float sala_consumption_mwh;

// Métricas de telemetria do Banheiro
extern float banheiro_on_time_s;
extern int banheiro_activations;
extern float banheiro_consumption_mwh;

// Métricas de telemetria da Varanda
extern float varanda_on_time_s;
extern int varanda_activations;
extern float varanda_consumption_mwh;

// Qualidade do sinal Wi-Fi (dBm)
extern int wifi_rssi_dbm;

/**
 * @brief Registra as propriedades no Thing ID e configura a conexão Wi-Fi.
 */
void init_cloud_properties();

/**
 * @brief Callback executado ao alterar o estado da Cozinha via Nuvem/Alexa.
 */
void on_cozinha_change();

/**
 * @brief Callback executado ao alterar o estado da Sala via Nuvem/Alexa.
 */
void on_sala_change();

/**
 * @brief Callback executado ao alterar o estado do Banheiro via Nuvem/Alexa.
 */
void on_banheiro_change();

/**
 * @brief Callback executado ao alterar o estado da Varanda via Nuvem/Alexa.
 */
void on_varanda_change();

// Gerenciador de conexão de rede preferencial
extern WiFiConnectionHandler ArduinoIoTPreferredConnection;
