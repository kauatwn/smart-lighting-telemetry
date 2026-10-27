/**
 * @file thing_properties.cpp
 * @brief Define e registra as propriedades da Arduino IoT Cloud.
 *
 * As credenciais de conexão e o identificador do Thing são obtidos
 * de include/secrets.h.
 */

#include "thing_properties.h"

#include "secrets.h"

// Controles remotos dos pontos de iluminação.
CloudSwitch cozinha;
CloudSwitch sala;
CloudSwitch banheiro;
CloudSwitch varanda;

// Métricas da cozinha.
float cozinha_on_time_s = 0.0F;
int cozinha_activations = 0;
float cozinha_consumption_mwh = 0.0F;

// Métricas da sala.
float sala_on_time_s = 0.0F;
int sala_activations = 0;
float sala_consumption_mwh = 0.0F;

// Métricas do banheiro.
float banheiro_on_time_s = 0.0F;
int banheiro_activations = 0;
float banheiro_consumption_mwh = 0.0F;

// Métricas da varanda.
float varanda_on_time_s = 0.0F;
int varanda_activations = 0;
float varanda_consumption_mwh = 0.0F;

// Intensidade do sinal Wi-Fi em dBm.
int wifi_rssi_dbm = -127;

void init_cloud_properties() {
  ArduinoCloud.setThingId(config::thing_id);

  // Controles remotos da iluminação
  ArduinoCloud.addProperty(cozinha, READWRITE, ON_CHANGE, on_cozinha_change);
  ArduinoCloud.addProperty(sala, READWRITE, ON_CHANGE, on_sala_change);
  ArduinoCloud.addProperty(banheiro, READWRITE, ON_CHANGE, on_banheiro_change);
  ArduinoCloud.addProperty(varanda, READWRITE, ON_CHANGE, on_varanda_change);

  // Cozinha
  ArduinoCloud.addProperty(cozinha_on_time_s, READ, 5 * SECONDS, nullptr);
  ArduinoCloud.addProperty(cozinha_activations, READ, ON_CHANGE, nullptr);
  ArduinoCloud.addProperty(cozinha_consumption_mwh, READ, 5 * SECONDS, nullptr);

  // Sala
  ArduinoCloud.addProperty(sala_on_time_s, READ, 5 * SECONDS, nullptr);
  ArduinoCloud.addProperty(sala_activations, READ, ON_CHANGE, nullptr);
  ArduinoCloud.addProperty(sala_consumption_mwh, READ, 5 * SECONDS, nullptr);

  // Banheiro
  ArduinoCloud.addProperty(banheiro_on_time_s, READ, 5 * SECONDS, nullptr);
  ArduinoCloud.addProperty(banheiro_activations, READ, ON_CHANGE, nullptr);
  ArduinoCloud.addProperty(banheiro_consumption_mwh, READ, 5 * SECONDS, nullptr);

  // Varanda
  ArduinoCloud.addProperty(varanda_on_time_s, READ, 5 * SECONDS, nullptr);
  ArduinoCloud.addProperty(varanda_activations, READ, ON_CHANGE, nullptr);
  ArduinoCloud.addProperty(varanda_consumption_mwh, READ, 5 * SECONDS, nullptr);

  // Monitoramento da conexão Wi-Fi
  ArduinoCloud.addProperty(wifi_rssi_dbm, READ, 5 * SECONDS, nullptr);
}

// Configura as credenciais da conexão Wi-Fi
WiFiConnectionHandler ArduinoIoTPreferredConnection(config::wifi_ssid, config::wifi_pass);
