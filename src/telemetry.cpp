/**
 * @file telemetry.cpp
 * @brief Implementação das métricas operacionais.
 *
 * Calcula o tempo ligado, conta os acionamentos e estima a energia
 * consumida por cada ponto de iluminação. Também monitora o sinal
 * Wi-Fi periodicamente.
 *
 * A energia é estimada a partir de uma potência nominal configurável;
 * não representa uma medição elétrica real.
 */

#include "telemetry.h"

#include <Arduino.h>
#include <WiFiS3.h>

#include "lighting.h"
#include "thing_properties.h"

namespace telemetry {
namespace {
// Potência assumida para cada ponto de iluminação.
constexpr float LIGHT_POWER_W = 9.0F;

// Intervalo entre consultas da intensidade do sinal Wi-Fi.
constexpr unsigned long RSSI_UPDATE_INTERVAL_MS = 5000;

// Valor utilizado quando o dispositivo não está conectado.
constexpr int WIFI_DISCONNECTED_RSSI_DBM = -127;

constexpr uint8_t CHANNEL_COUNT = lighting::CHANNEL_COUNT;

struct ChannelMetrics {
  float* on_time_s;
  int* activations;
  float* consumption_mwh;
};

// Apenas declaramos o array global sem inicializar com os ponteiros externos
ChannelMetrics metrics[CHANNEL_COUNT];

bool previous_states[CHANNEL_COUNT] = {};
unsigned long last_update_ms = 0;
unsigned long last_rssi_update_ms = 0;
}  // namespace

void init(const unsigned long start_ms) {
  last_update_ms = start_ms;
  last_rssi_update_ms = start_ms;

  // Atribuição dos ponteiros em tempo de execução dentro do setup()
  metrics[0] = {
      .on_time_s = &cozinha_on_time_s,
      .activations = &cozinha_activations,
      .consumption_mwh = &cozinha_consumption_mwh,
  };
  metrics[1] = {
      .on_time_s = &sala_on_time_s,
      .activations = &sala_activations,
      .consumption_mwh = &sala_consumption_mwh,
  };
  metrics[2] = {
      .on_time_s = &banheiro_on_time_s,
      .activations = &banheiro_activations,
      .consumption_mwh = &banheiro_consumption_mwh,
  };
  metrics[3] = {
      .on_time_s = &varanda_on_time_s,
      .activations = &varanda_activations,
      .consumption_mwh = &varanda_consumption_mwh,
  };

  for (uint8_t channel = 0; channel < CHANNEL_COUNT; ++channel) {
    previous_states[channel] = lighting::get_channel(channel);

    *metrics[channel].on_time_s = 0.0F;
    *metrics[channel].activations = 0;
    *metrics[channel].consumption_mwh = 0.0F;
  }

  wifi_rssi_dbm = WIFI_DISCONNECTED_RSSI_DBM;
}

void update(const unsigned long current_ms) {
  const unsigned long delta_ms = current_ms - last_update_ms;
  last_update_ms = current_ms;

  for (uint8_t channel = 0; channel < CHANNEL_COUNT; ++channel) {
    const bool is_on = lighting::get_channel(channel);

    // Conta apenas transições de desligado para ligado.
    if (is_on && !previous_states[channel]) {
      ++(*metrics[channel].activations);
    }

    if (is_on) {
      *metrics[channel].on_time_s += static_cast<float>(delta_ms) / 1000.0F;

      // Converte potência × tempo para miliwatt-hora (mWh).
      *metrics[channel].consumption_mwh += LIGHT_POWER_W * static_cast<float>(delta_ms) / 3600.0F;
    }

    previous_states[channel] = is_on;
  }

  // Consulta o sinal periodicamente sem bloquear o loop principal.
  if (current_ms - last_rssi_update_ms >= RSSI_UPDATE_INTERVAL_MS) {
    last_rssi_update_ms = current_ms;

    wifi_rssi_dbm = WiFi.status() == WL_CONNECTED ? static_cast<int>(WiFi.RSSI()) : WIFI_DISCONNECTED_RSSI_DBM;
  }
}
}  // namespace telemetry
