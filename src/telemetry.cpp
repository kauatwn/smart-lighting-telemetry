/*
 * Telemetry Implementation - Smart Lighting Telemetry
 *
 * Descrição:
 * Coleta procedural de telemetria de hardware através de amostragem por registradores
 * do sensor de silício do Renesas RA4M1, sinal Wi-Fi e tempo de atividade.
 */

#include "telemetry.h"

#include <WiFiS3.h>
#include <bsp_api.h>

namespace telemetry {

namespace {
constexpr float fallback_temperature_c = 28.0F;

// Realiza a leitura física direta do registrador ADTSDR do Renesas RA4M1
float read_raw_ra4m1_temperature() {
  // Dispara a conversão A/D no canal do sensor interno de temperatura
  R_ADC0->ADCSR_b.ADST = 1;

  uint32_t timeout = 1000;
  while (R_ADC0->ADCSR_b.ADST == 1 && --timeout > 0) {
  }

  const uint16_t raw_counts = R_ADC0->ADTSDR;
  if (raw_counts == 0 || raw_counts >= 16383) {
    return fallback_temperature_c;
  }

  // Conversão para milivolts considerando ADC de 14 bits (16384 passos) e VREF de 5V
  const float sensor_mv = static_cast<float>(raw_counts) * 5000.0F / 16384.0F;

  // Característica térmica do sensor TSN do RA4M1: ~1050 mV a 25 °C com inclinação de -3.65 mV/°C
  const float calculated_temp = 25.0F + (1050.0F - sensor_mv) / 3.65F;

  // Filtro de plausibilidade para die temperature (-10 °C a 85 °C)
  if (calculated_temp < -10.0F || calculated_temp > 85.0F) {
    return fallback_temperature_c;
  }

  return calculated_temp;
}
}  // namespace

void init() {
  // Habilita o clock do sensor de temperatura no registrador de controle de módulos (MSTP)
  R_MSTP->MSTPCRC &= ~(1UL << 14U);

  // Seleciona o sensor interno de temperatura para o conversor A/D (ADC140)
  R_ADC0->ADEXICR_b.TSSA = 1;
}

int read_wifi_rssi() {
  const auto rssi = static_cast<int>(WiFi.RSSI());
  if (rssi == 0 || rssi < -120 || rssi > 0) {
    return -100;
  }
  return rssi;
}

float read_temperature_c(const uint8_t active_led_count, const uint8_t brightness_pct) {
  const float die_temp = read_raw_ra4m1_temperature();

  // Incorpora o delta térmico causado pelo acionamento dos pinos de saída PWM
  const float dissipation_factor = static_cast<float>(active_led_count) * static_cast<float>(brightness_pct) / 400.0F;
  return die_temp + dissipation_factor * 1.5F;
}

int get_uptime_minutes(const unsigned long current_ms) {
  constexpr unsigned long ms_per_minute = 60000;
  return static_cast<int>(current_ms / ms_per_minute);
}

String get_status_message(const int rssi_dbm, const float temp_c, const bool is_alert_mode) {
  if (is_alert_mode) {
    return F("ALERTA ATIVO - Modo Estrobo");
  }

  if (rssi_dbm < -80) {
    return F("Sinal Wi-Fi Fraco");
  }

  if (temp_c > 45.0F) {
    return F("Atenção: Temperatura Elevada");
  }

  return F("Sistema Operacional - Normal");
}

}  // namespace telemetry
