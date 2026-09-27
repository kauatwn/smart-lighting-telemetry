/*
 * Main Application - Smart Lighting Telemetry
 *
 * Descrição do Projeto:
 * Firmware para controle de iluminação em 4 zonas e monitoramento em tempo real
 * com Arduino Uno R4 WiFi integrado ao Arduino Cloud e matriz de 12x8 LEDs.
 *
 * Funcionalidades:
 * - Controle individual liga/desliga de 4 canais de iluminação com modulação PWM.
 * - Modos de operação: Manual, Todas Ligadas, Efeito Respiração (Fade) e Alerta.
 * - Matriz 12x8 nativa: Ícones de Sol (Modo Ativo), Lua (Modo Noturno) e Lâmpada (Brilho).
 * - Telemetria contínua: temperatura do chip RA4M1, sinal Wi-Fi (RSSI) e tempo ativo.
 */

#include <Arduino.h>

#include <algorithm>

#include "lighting.h"
#include "matrix_display.h"
#include "telemetry.h"
#include "thing_properties.h"

namespace {
constexpr unsigned long serial_baud_rate = 115200;
constexpr unsigned long telemetry_interval_ms = 3000;
constexpr uint8_t telemetry_heartbeat_interval = 4;  // Pulso de heartbeat a cada 4 leituras (12 segundos)

unsigned long last_telemetry_ms = 0;
uint8_t telemetry_cycles = 0;

uint8_t count_active_lights() {
  uint8_t count = 0;
  for (uint8_t i = 0; i < lighting::channel_count; ++i) {
    if (lighting::get_channel(i)) {
      ++count;
    }
  }
  return count;
}

void on_cloud_connect() {
  Serial.println(F("[CLOUD] Conectado com sucesso ao Arduino IoT Cloud!"));
  matrix_display::show_check_success();
}

void on_cloud_disconnect() {
  Serial.println(F("[CLOUD] Conexao perdida. Buscando rede Wi-Fi..."));
  matrix_display::show_wifi_searching();
}

void update_telemetry(const unsigned long current_ms, const uint8_t active_count, const bool is_alert_mode) {
  if (current_ms - last_telemetry_ms < telemetry_interval_ms) {
    return;
  }
  last_telemetry_ms = current_ms;

  const uint8_t current_brightness = lighting::get_brightness();

  wifi_rssi = telemetry::read_wifi_rssi();
  uptime_minutes = telemetry::get_uptime_minutes(current_ms);
  temperature_c = telemetry::read_temperature_c(active_count, current_brightness);
  status_message = telemetry::get_status_message(wifi_rssi, temperature_c, is_alert_mode);

  if (is_alert_mode) {
    return;
  }

  if (++telemetry_cycles < telemetry_heartbeat_interval) {
    return;
  }

  telemetry_cycles = 0;
  matrix_display::show_heartbeat();
}
}  // namespace

void setup() {
  Serial.begin(serial_baud_rate);
  delay(1000);

  Serial.println(F("=================================================="));
  Serial.println(F(" SMART LIGHTING TELEMETRY - ARDUINO UNO R4 WIFI   "));
  Serial.println(F(" Cloud: Arduino IoT Cloud (Maker Plan)            "));
  Serial.println(F(" Hardware: 4x LED Channels (PWM) + 12x8 Matrix    "));
  Serial.println(F("=================================================="));

  lighting::init();
  matrix_display::init();
  telemetry::init();

  // Inicia animação de busca Wi-Fi na matriz durante a conexão
  matrix_display::show_wifi_searching();

  init_cloud_properties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  // Registra callbacks de conectividade para atualizar o display visual
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::CONNECT, on_cloud_connect);
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::DISCONNECT, on_cloud_disconnect);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  Serial.println(F("[SYSTEM] Inicializacao concluida. Entrando no loop principal."));
  Serial.println(F("=================================================="));
}

void loop() {
  const unsigned long current_ms = millis();
  const uint8_t active_count = count_active_lights();
  const bool is_alert_mode = (operation_mode == 3);

  ArduinoCloud.update();
  lighting::update(current_ms);
  matrix_display::update(active_count, is_alert_mode);
  update_telemetry(current_ms, active_count, is_alert_mode);
}

void on_light1_change() {
  lighting::set_channel(0, light1);
  Serial.print(F("[CLOUD] Luz 1 alterada: "));
  Serial.println(light1 ? F("LIGADA") : F("DESLIGADA"));
}

void on_light2_change() {
  lighting::set_channel(1, light2);
  Serial.print(F("[CLOUD] Luz 2 alterada: "));
  Serial.println(light2 ? F("LIGADA") : F("DESLIGADA"));
}

void on_light3_change() {
  lighting::set_channel(2, light3);
  Serial.print(F("[CLOUD] Luz 3 alterada: "));
  Serial.println(light3 ? F("LIGADA") : F("DESLIGADA"));
}

void on_light4_change() {
  lighting::set_channel(3, light4);
  Serial.print(F("[CLOUD] Luz 4 alterada: "));
  Serial.println(light4 ? F("LIGADA") : F("DESLIGADA"));
}

void on_brightness_change() {
  brightness_pct = std::clamp(brightness_pct, 0, 100);

  lighting::set_brightness(static_cast<uint8_t>(brightness_pct));
  matrix_display::show_brightness_indicator();

  Serial.print(F("[CLOUD] Brilho geral alterado: "));
  Serial.print(brightness_pct);
  Serial.println(F("%"));
}

void on_operation_mode_change() {
  if (operation_mode < 0 || operation_mode > 3) {
    operation_mode = 0;
  }

  lighting::set_mode(static_cast<lighting::OperationMode>(operation_mode));
  Serial.print(F("[CLOUD] Modo de operacao alterado para: "));
  Serial.println(operation_mode);
}