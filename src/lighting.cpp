/*
 * Lighting Controller Implementation - Smart Lighting Telemetry
 *
 * Descrição:
 * Implementação procedural do acionamento físico dos LEDs com suporte a PWM,
 * controle individual de canais e efeitos de iluminação não-bloqueantes.
 */

#include "lighting.h"

#include <Arduino.h>
#include <math.h>

#include <algorithm>

namespace lighting {
namespace {
constexpr uint8_t led_pins[channel_count] = {3, 5, 6, 9};

bool channel_states[channel_count] = {false, false, false, false};
uint8_t current_brightness = 100;
auto current_mode = OperationMode::MODE_MANUAL;

void apply_pwm(const uint8_t pin, const uint8_t duty_cycle) { analogWrite(pin, duty_cycle); }

uint8_t pct_to_pwm(uint8_t pct) {
  pct = std::min<uint8_t>(pct, 100);
  return static_cast<uint8_t>(static_cast<uint16_t>(pct) * 255 / 100);
}
}  // namespace

void init() {
  for (const uint8_t pin : led_pins) {
    pinMode(pin, OUTPUT);
    apply_pwm(pin, 0);
  }
}

void set_channel(const uint8_t channel_index, const bool state) {
  if (channel_index >= channel_count) {
    return;
  }
  channel_states[channel_index] = state;
}

bool get_channel(const uint8_t channel_index) {
  if (channel_index >= channel_count) {
    return false;
  }
  return channel_states[channel_index];
}

void set_brightness(const uint8_t brightness_pct) { current_brightness = brightness_pct > 100 ? 100 : brightness_pct; }

uint8_t get_brightness() { return current_brightness; }

void set_mode(const OperationMode mode) { current_mode = mode; }

void update(const unsigned long current_ms) {
  const uint8_t base_pwm = pct_to_pwm(current_brightness);

  switch (current_mode) {
    case OperationMode::MODE_MANUAL: {
      for (uint8_t i = 0; i < channel_count; ++i) {
        apply_pwm(led_pins[i], channel_states[i] ? base_pwm : 0);
      }
      break;
    }
    case OperationMode::MODE_ALL_ON: {
      for (const uint8_t pin : led_pins) {
        apply_pwm(pin, base_pwm);
      }
      break;
    }
    case OperationMode::MODE_BREATHE: {
      constexpr unsigned long period_ms = 3000;
      const unsigned long cycle_pos = current_ms % period_ms;
      const float angle = static_cast<float>(cycle_pos) / period_ms * 2.0F * static_cast<float>(M_PI);
      const float normalized = (sinf(angle - static_cast<float>(M_PI) / 2.0F) + 1.0F) / 2.0F;
      const auto breathe_pwm = static_cast<uint8_t>(normalized * static_cast<float>(base_pwm));

      for (const uint8_t pin : led_pins) {
        apply_pwm(pin, breathe_pwm);
      }
      break;
    }
    case OperationMode::MODE_ALERT: {
      constexpr unsigned long strobe_interval_ms = 150;
      const bool strobe_on = current_ms / strobe_interval_ms % 2 == 0;
      const uint8_t alert_pwm = strobe_on ? 255 : 0;

      for (const uint8_t pin : led_pins) {
        apply_pwm(pin, alert_pwm);
      }
      break;
    }
  }
}
}  // namespace lighting
