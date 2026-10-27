/**
 * @file lighting.cpp
 * @brief Implementação do controle de iluminação e do relé.
 *
 * Inicializa e controla os quatro LEDs, o relé e o botão físico.
 * Mantém o estado lógico das saídas e detecta o acionamento do
 * botão com debounce não bloqueante.
 *
 * Hardware: Arduino Uno R4 WiFi.
 */

#include "lighting.h"

#include <Arduino.h>

namespace lighting {
namespace {

// Define os pinos de saída para cada canal de iluminação.
constexpr uint8_t PIN_COZINHA = 6;
constexpr uint8_t PIN_SALA = 3;
constexpr uint8_t PIN_BANHEIRO = 4;
constexpr uint8_t PIN_VARANDA = 5;

constexpr uint8_t LED_PINS[CHANNEL_COUNT] = {
    PIN_COZINHA,
    PIN_SALA,
    PIN_BANHEIRO,
    PIN_VARANDA,
};

constexpr uint8_t RELAY_PIN = 9;
constexpr uint8_t BUTTON_PIN = 12;

// O módulo de relé é acionado em nível LOW.
constexpr uint8_t RELAY_ON = LOW;
constexpr uint8_t RELAY_OFF = HIGH;

constexpr unsigned long DEBOUNCE_DELAY_MS = 50;

bool channel_states[CHANNEL_COUNT] = {false, false, false, false};
bool relay_state = false;

// Estados utilizados para filtrar oscilações do botão.
bool last_button_reading = HIGH != 0U;
bool stable_button_state = HIGH != 0U;
unsigned long last_debounce_ms = 0;
}  // namespace

void init() {
  // Inicializa os LEDs desligados.
  for (uint8_t channel = 0; channel < CHANNEL_COUNT; ++channel) {
    pinMode(LED_PINS[channel], OUTPUT);
    digitalWrite(LED_PINS[channel], LOW);
    channel_states[channel] = false;
  }

  // Define o estado inicial do relé como desligado.
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_OFF);
  relay_state = false;

  // O botão conecta o pino ao GND quando pressionado.
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  last_button_reading = HIGH != 0U;
  stable_button_state = HIGH != 0U;
  last_debounce_ms = millis();
}

void set_channel(const Channel channel, const bool enabled) {
  const auto index = static_cast<uint8_t>(channel);

  if (index >= CHANNEL_COUNT) {
    return;
  }

  // Evita escritas desnecessárias no hardware.
  if (channel_states[index] == enabled) {
    return;
  }

  digitalWrite(LED_PINS[index], enabled ? HIGH : LOW);
  channel_states[index] = enabled;
}

bool get_channel(const Channel channel) {
  const auto index = static_cast<uint8_t>(channel);

  if (index >= CHANNEL_COUNT) {
    return false;
  }

  return channel_states[index];
}

void set_relay(const bool enabled) {
  if (relay_state == enabled) {
    return;
  }

  digitalWrite(RELAY_PIN, enabled ? RELAY_ON : RELAY_OFF);
  relay_state = enabled;
}

bool get_relay_state() { return relay_state; }

bool process_physical_button(const unsigned long current_ms) {
  const bool reading = digitalRead(BUTTON_PIN) != 0U;
  bool relay_toggled = false;

  // Reinicia a contagem sempre que a leitura elétrica mudar.
  if (reading != last_button_reading) {
    last_debounce_ms = current_ms;
  }

  // Aceita a leitura somente após permanecer estável.
  if (current_ms - last_debounce_ms >= DEBOUNCE_DELAY_MS) {
    if (reading != stable_button_state) {
      stable_button_state = reading;

      // INPUT_PULLUP: LOW indica que o botão foi pressionado.
      if (static_cast<int>(stable_button_state) == LOW) {
        set_relay(!relay_state);
        relay_toggled = true;
      }
    }
  }

  last_button_reading = reading;

  return relay_toggled;
}
}  // namespace lighting
