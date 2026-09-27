/*
 * Matrix Display Implementation - Smart Lighting Telemetry
 *
 * Descrição:
 * Implementação do controle visual da matriz de 12x8 LEDs integrada do
 * Arduino Uno R4 WiFi utilizando exclusivamente ícones e animações nativas
 * da galeria oficial (gallery.h), sem frames customizados.
 */

#include "matrix_display.h"

#include <Arduino.h>
#include <Arduino_LED_Matrix.h>

namespace matrix_display {

namespace {
ArduinoLEDMatrix matrix;

enum class State : uint8_t {
  UNINITIALIZED,
  WIFI_SEARCHING,
  CHECK_SUCCESS,
  HEARTBEAT,
  BRIGHTNESS_INDICATOR,
  DANGER,
  LIGHTS_ACTIVE,
  LIGHTS_OFF,
};

auto current_state = State::UNINITIALIZED;
unsigned long overlay_until_ms = 0;
constexpr unsigned long brightness_display_duration_ms = 2500;
constexpr unsigned long check_display_duration_ms = 1500;
constexpr unsigned long heartbeat_duration_ms = 1500;
}  // namespace

void init() {
  matrix.begin();
  current_state = State::UNINITIALIZED;
}

void show_wifi_searching() {
  current_state = State::WIFI_SEARCHING;
  overlay_until_ms = 0;
  matrix.loadSequence(LEDMATRIX_ANIMATION_WIFI_SEARCH);
  matrix.play(true);
}

void show_check_success() {
  current_state = State::CHECK_SUCCESS;
  overlay_until_ms = millis() + check_display_duration_ms;
  matrix.loadSequence(LEDMATRIX_ANIMATION_CHECK);
  matrix.play(false);
}

void show_heartbeat() {
  if (current_state == State::WIFI_SEARCHING || current_state == State::DANGER) {
    return;
  }
  current_state = State::HEARTBEAT;
  overlay_until_ms = millis() + heartbeat_duration_ms;
  matrix.loadSequence(LEDMATRIX_ANIMATION_HEARTBEAT);
  matrix.play(false);
}

void show_danger() {
  if (current_state == State::DANGER) {
    return;
  }
  current_state = State::DANGER;
  overlay_until_ms = 0;
  matrix.loadFrame(LEDMATRIX_DANGER);
}

void show_brightness_indicator() {
  if (current_state == State::WIFI_SEARCHING) {
    return;
  }
  current_state = State::BRIGHTNESS_INDICATOR;
  overlay_until_ms = millis() + brightness_display_duration_ms;
  matrix.loadSequence(LEDMATRIX_ANIMATION_BATTERY);
  matrix.play(false);
}

void update(const uint8_t active_lights_count, const bool is_alert_mode) {
  const unsigned long current_ms = millis();

  // Modo Alerta tem prioridade absoluta enquanto estiver ativo
  if (is_alert_mode) {
    show_danger();
    return;
  }

  // Se acabou de sair do modo Alerta, reseta para reavaliar o estado base
  if (current_state == State::DANGER) {
    current_state = State::UNINITIALIZED;
  }

  // Se houver uma sobreposição temporária (Check, Heartbeat, Bateria) em exibição:
  if (current_ms < overlay_until_ms) {
    return;
  }

  if (overlay_until_ms > 0) {
    overlay_until_ms = 0;
    current_state = State::UNINITIALIZED;
  }

  // Se ainda estiver na animação de busca Wi-Fi inicial, não sobrepõe
  if (current_state == State::WIFI_SEARCHING) {
    return;
  }

  // Exibição base conforme as luzes ativas (Sad = 0 luzes, Happy >= 1 luz)
  const State desired_state = active_lights_count == 0 ? State::LIGHTS_OFF : State::LIGHTS_ACTIVE;
  if (current_state == desired_state) {
    return;
  }

  current_state = desired_state;
  const uint32_t* target_frame = (desired_state == State::LIGHTS_OFF) ? LEDMATRIX_EMOJI_SAD : LEDMATRIX_EMOJI_HAPPY;
  matrix.loadFrame(target_frame);
}

}  // namespace matrix_display
