/**
 * @file telemetry.cpp
 * @brief Implementação do cálculo de telemetria operacional sem bloqueio do loop.
 *
 * Realiza a integração temporal para estimativa do consumo elétrico em mWh
 * baseado em uma carga nominal constante de 9W por canal aceso e contagem de
 * ativações via detecção de borda de subida.
 *
 * Hardware: Arduino Uno R4 WiFi.
 */

#include "telemetry.h"

#include <Arduino.h>

#include "lighting.h"

namespace telemetry {

namespace {

// Potência nominal assumida para cada ponto de iluminação (9W por canal).
constexpr float LIGHT_POWER_W = 9.0F;

ChannelMetrics bound_metrics[lighting::CHANNEL_COUNT];
bool previous_states[lighting::CHANNEL_COUNT] = {false, false, false, false};
unsigned long last_update_ms = 0;

}  // namespace

void init(const ChannelMetrics metrics[lighting::CHANNEL_COUNT], const unsigned long start_ms) {
  for (uint8_t index = 0; index < lighting::CHANNEL_COUNT; ++index) {
    bound_metrics[index] = metrics[index];

    const auto channel = static_cast<lighting::Channel>(index);
    previous_states[index] = lighting::get_channel(channel);
  }

  last_update_ms = start_ms;
}

void update(const unsigned long current_ms) {
  const unsigned long delta_ms = current_ms - last_update_ms;
  last_update_ms = current_ms;

  const float delta_seconds = static_cast<float>(delta_ms) / 1000.0F;

  for (uint8_t index = 0; index < lighting::CHANNEL_COUNT; ++index) {
    const auto channel = static_cast<lighting::Channel>(index);

    // Guard Clause: se estiver desligado, registra o estado e pula para a próxima iteração
    if (const bool is_on = lighting::get_channel(channel); !is_on) {
      previous_states[index] = false;
      continue;
    }

    // Detecção de borda de subida: transição de desligado para ligado
    if (!previous_states[index] && bound_metrics[index].activations != nullptr) {
      *bound_metrics[index].activations += 1;
    }

    // Acúmulo de tempo e consumo energético se o canal estiver aceso
    if (bound_metrics[index].on_time_s != nullptr) {
      *bound_metrics[index].on_time_s += delta_seconds;
    }

    if (bound_metrics[index].consumption_mwh != nullptr) {
      // Conversão física: mWh = Potência (W) * tempo (ms) / 3600 (s/h)
      const float delta_mwh = LIGHT_POWER_W * static_cast<float>(delta_ms) / 3600.0F;
      *bound_metrics[index].consumption_mwh += delta_mwh;
    }

    previous_states[index] = true;
  }
}

void reset() {
  for (auto& [on_time_s, consumption_mwh, activations] : bound_metrics) {
    if (on_time_s != nullptr) {
      *on_time_s = 0.0F;
    }

    if (consumption_mwh != nullptr) {
      *consumption_mwh = 0.0F;
    }

    if (activations != nullptr) {
      *activations = 0;
    }
  }
}

}  // namespace telemetry
