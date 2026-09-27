/*
 * Lighting Controller Module - Smart Lighting Telemetry
 *
 * Descrição:
 * Declaração das rotinas de controle procedural para 4 canais de iluminação LED,
 * suportando modulação PWM de brilho e modos de operação com animações não-bloqueantes.
 */

#pragma once

#include <stdint.h>

namespace lighting {

constexpr uint8_t channel_count = 4;

enum class OperationMode : uint8_t {
  MODE_MANUAL = 0,
  MODE_ALL_ON = 1,
  MODE_BREATHE = 2,
  MODE_ALERT = 3,
};

// Inicializa os pinos de saída PWM e desliga os LEDs
void init();

// Define o estado lógico de um canal individual (índice 0 a 3)
void set_channel(uint8_t channel_index, bool state);

// Retorna o estado lógico atual de um canal individual
bool get_channel(uint8_t channel_index);

// Define o percentual de brilho geral (0 a 100%)
void set_brightness(uint8_t brightness_pct);

// Retorna o brilho configurado atualmente
uint8_t get_brightness();

// Define o modo de operação do sistema de iluminação
void set_mode(OperationMode mode);

// Atualiza o estado físico e executa animações temporizadas com millis()
void update(unsigned long current_ms);

}  // namespace lighting
