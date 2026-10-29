/**
 * @file main.cpp
 * @brief Ponto de entrada e orquestrador principal do sistema de iluminação residencial.
 *
 * Coordena a comunicação remota com a Arduino IoT Cloud/Alexa, a leitura do botão
 * físico, o diagnóstico visual na matriz de LEDs e a atualização periódica da telemetria.
 *
 * Hardware: Arduino Uno R4 WiFi.
 */

#include <Arduino.h>
#include <ArduinoIoTCloud.h>

#include "lighting.h"
#include "matrix_display.h"
#include "telemetry.h"
#include "thing_properties.h"

namespace {

/**
 * @brief Callback ativado quando a placa estabelece conexão com a nuvem.
 */
void on_cloud_connect() {
  Serial.println(F("[CLOUD] Conectado e pronto para a Alexa."));
  matrix_display::show_check_success();
}

/**
 * @brief Callback ativado quando ocorre perda de conexão com a nuvem.
 */
void on_cloud_disconnect() {
  Serial.println(F("[CLOUD] Desconectado. Aguardando reconexão com a rede Wi-Fi..."));
  matrix_display::show_wifi_searching();
}

/**
 * @brief Processa as entradas do hardware (botão físico) e sincroniza com a nuvem.
 * @param current_ms Tempo atual do sistema fornecido por millis().
 */
void process_hardware_inputs(const unsigned long current_ms) {
  if (lighting::process_physical_button(current_ms)) {
    const bool enabled = lighting::get_relay_state();
    cozinha = enabled;
    lighting::set_channel(lighting::Channel::COZINHA, enabled);
  }
}

/**
 * @brief Configura a injeção de dependência dos ponteiros de telemetria.
 * @param start_ms Tempo inicial obtido via millis().
 */
void setup_telemetry(const unsigned long start_ms) {
  constexpr telemetry::ChannelMetrics metrics[lighting::CHANNEL_COUNT] = {
      {
          .on_time_s = &cozinha_on_time_s,
          .consumption_mwh = &cozinha_consumption_mwh,
          .activations = &cozinha_activations,
      },
      {
          .on_time_s = &sala_on_time_s,
          .consumption_mwh = &sala_consumption_mwh,
          .activations = &sala_activations,
      },
      {
          .on_time_s = &banheiro_on_time_s,
          .consumption_mwh = &banheiro_consumption_mwh,
          .activations = &banheiro_activations,
      },
      {
          .on_time_s = &varanda_on_time_s,
          .consumption_mwh = &varanda_consumption_mwh,
          .activations = &varanda_activations,
      },
  };

  telemetry::init(metrics, start_ms);
}

}  // namespace

void setup() {
  Serial.begin(115200);

  // Inicialização dos módulos do sistema
  lighting::init();
  matrix_display::init();
  setup_telemetry(millis());

  // Inicialização da nuvem
  init_cloud_properties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  // Registro dos callbacks de evento de conexão
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::CONNECT, on_cloud_connect);
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::DISCONNECT, on_cloud_disconnect);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  Serial.println(F("Sistema de iluminação inicializado com sucesso."));
}

void loop() {
  const unsigned long current_ms = millis();

  // Atualizações de estado do sistema sem bloqueio
  ArduinoCloud.update();
  matrix_display::update(current_ms);

  process_hardware_inputs(current_ms);
  telemetry::update(current_ms);
}

// Callbacks de Comandos Remotos (Arduino Cloud / Amazon Alexa)
void on_cozinha_change() {
  const bool state = cozinha;
  lighting::set_channel(lighting::Channel::COZINHA, state);
  lighting::set_relay(state);
}

void on_sala_change() { lighting::set_channel(lighting::Channel::SALA, sala); }

void on_banheiro_change() { lighting::set_channel(lighting::Channel::BANHEIRO, banheiro); }

void on_varanda_change() { lighting::set_channel(lighting::Channel::VARANDA, varanda); }
