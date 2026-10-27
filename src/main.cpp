/**
 * @file main.cpp
 * @brief Ponto de entrada do sistema de iluminação inteligente.
 *
 * Inicializa o hardware, a matriz de LEDs, a telemetria e a
 * Arduino IoT Cloud. Coordena a comunicação remota, o botão
 * físico e a atualização das métricas operacionais.
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
void on_cloud_connect() {
  Serial.println(F("[CLOUD] Conectado e pronto para a Alexa."));
  matrix_display::show_check_success();
}

void on_cloud_disconnect() {
  Serial.println(F("[CLOUD] Desconectado. Buscando rede Wi-Fi..."));
  matrix_display::show_wifi_searching();
}
}  // namespace

/**
 * @brief Inicializa o hardware e os serviços do sistema.
 */
void setup() {
  Serial.begin(115200);

  lighting::init();
  matrix_display::init();
  telemetry::init(millis());

  init_cloud_properties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::CONNECT, on_cloud_connect);
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::DISCONNECT, on_cloud_disconnect);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  Serial.println(F("Sistema de iluminacao inicializado."));
}

/**
 * @brief Processa a comunicação, a matriz, o botão e a telemetria.
 */
void loop() {
  const unsigned long current_ms = millis();

  ArduinoCloud.update();
  matrix_display::update();

  // Se o botão físico no pino 12 for pressionado:
  if (lighting::process_physical_button(current_ms)) {
    const bool estado_rele = lighting::get_relay_state();

    // Atualiza a nuvem/Alexa e o LED da cozinha com o novo estado do relé
    cozinha = estado_rele;
    lighting::set_channel(lighting::Channel::COZINHA, estado_rele);
  }

  telemetry::update(millis());
}

/**
 * @brief Aplica ao hardware uma alteração remota da cozinha (LED + Relé).
 */
void on_cozinha_change() {
  lighting::set_channel(lighting::Channel::COZINHA, cozinha);
  lighting::set_relay(cozinha);
}

/**
 * @brief Aplica ao hardware uma alteração remota da sala.
 */
void on_sala_change() { lighting::set_channel(lighting::Channel::SALA, sala); }

/**
 * @brief Aplica ao hardware uma alteração remota do banheiro.
 */
void on_banheiro_change() { lighting::set_channel(lighting::Channel::BANHEIRO, banheiro); }

/**
 * @brief Aplica ao hardware uma alteração remota da varanda.
 */
void on_varanda_change() { lighting::set_channel(lighting::Channel::VARANDA, varanda); }
