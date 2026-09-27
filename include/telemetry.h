/*
 * Telemetry Module - Smart Lighting Telemetry
 *
 * Descrição:
 * Declaração das rotinas procedurais de coleta de métricas operacionais,
 * incluindo qualidade do sinal Wi-Fi (RSSI), temperatura do sistema e tempo de atividade.
 */

#pragma once

#include <Arduino.h>

namespace telemetry {

// Inicializa o subsistema de telemetria
void init();

// Realiza a leitura da intensidade de sinal Wi-Fi em dBm
int read_wifi_rssi();

// Realiza a leitura/estimativa da temperatura operacional do MCU em °C
float read_temperature_c(uint8_t active_led_count, uint8_t brightness_pct);

// Retorna o tempo de operação contínua do dispositivo em minutos
int get_uptime_minutes(unsigned long current_ms);

// Gera uma mensagem descritiva de status do sistema para o painel em nuvem
String get_status_message(int rssi_dbm, float temp_c, bool is_alert_mode);

}  // namespace telemetry
