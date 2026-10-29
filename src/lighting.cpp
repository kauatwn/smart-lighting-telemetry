/**
 * @file lighting.cpp
 * @brief Implementação do controle elétrico de saídas e entradas digitais.
 *
 * Mapeamento de pinos do Arduino Uno R4 WiFi:
 * - Cozinha (LED D6 + Relé D9)
 * - Sala (LED D3)
 * - Banheiro (LED D4)
 * - Varanda (LED D5)
 * - Botão Físico Cozinha (Pino D12 em INPUT_PULLUP)
 *
 * Hardware: Arduino Uno R4 WiFi.
 */

#include "lighting.h"

#include <Arduino.h>

namespace lighting {

namespace {

// Mapeamento dos pinos das saídas digitais dos LEDs
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

// O módulo relé de 5V opera em lógica invertida (LOW = acionado/fechado, HIGH = aberto)
constexpr uint8_t RELAY_PIN = 9;
constexpr uint8_t RELAY_ON = LOW;
constexpr uint8_t RELAY_OFF = HIGH;

// O botão físico é ligado ao GND (com pino em INPUT_PULLUP: LOW = pressionado)
constexpr uint8_t BUTTON_PIN = 12;
constexpr unsigned long DEBOUNCE_DELAY_MS = 50;

// Estados internos dos canais e periféricos
bool channel_states[CHANNEL_COUNT] = {false, false, false, false};
bool relay_state = false;

// Variáveis para controle do debounce temporal do botão
bool last_button_reading = HIGH != 0U;
bool stable_button_state = HIGH != 0U;
unsigned long last_debounce_ms = 0;

}  // namespace

void init() {
  // Configura os pinos dos LEDs como saída e garante início desligado
  for (uint8_t i = 0; i < CHANNEL_COUNT; ++i) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
    channel_states[i] = false;
  }

  // Configura o relé mantendo-o desligado na inicialização
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_OFF);
  relay_state = false;

  // Configura a entrada do botão com resistor pull-up interno ativo
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

  // Evita acessos redundantes ao barramento de hardware se o estado for idêntico
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

  // Reinicia a contagem se a leitura elétrica oscilar (ruído/ruído mecânico)
  if (reading != last_button_reading) {
    last_debounce_ms = current_ms;
  }

  // Confirma a transição apenas se o sinal elétrico se mantiver estável
  if (current_ms - last_debounce_ms >= DEBOUNCE_DELAY_MS) {
    if (reading != stable_button_state) {
      stable_button_state = reading;

      // INPUT_PULLUP: transição para LOW indica o clique do botão
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
