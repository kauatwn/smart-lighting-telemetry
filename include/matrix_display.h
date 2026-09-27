/*
 * Matrix Display Module - Smart Lighting Telemetry
 *
 * Descrição:
 * Declaração das rotinas de controle visual para a matriz de 12x8 LEDs integrada
 * no Arduino Uno R4 WiFi, utilizando 100% elementos nativos da galeria oficial
 * (Wi-Fi Search, Check, Heartbeat, Danger, Battery e Emojis Happy/Sad).
 */

#pragma once

#include <stdint.h>

namespace matrix_display {

// Inicializa a matriz de 12x8 LEDs integrada na placa
void init();

// Exibe a animação contínua de busca de rede Wi-Fi / Conexão Nuvem
void show_wifi_searching();

// Exibe a animação de sucesso (Checkmark) ao conectar
void show_check_success();

// Exibe a animação de batimento cardíaco (Heartbeat de telemetria)
void show_heartbeat();

// Exibe o ícone de perigo/alerta (Danger)
void show_danger();

// Exibe a animação da galeria de nível/bateria ao alterar o brilho
void show_brightness_indicator();

// Atualiza a exibição da matriz conforme o estado das luzes e modo de operação
void update(uint8_t active_lights_count, bool is_alert_mode = false);

}  // namespace matrix_display
