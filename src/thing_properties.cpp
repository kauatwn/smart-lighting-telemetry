/*
 * Cloud Properties Implementation - Smart Lighting Telemetry
 *
 * Descrição:
 * Instanciação das variáveis de estado do Arduino Cloud, registro das
 * propriedades na biblioteca oficial e configuração da conexão Wi-Fi.
 */

#include "thing_properties.h"

bool light1 = false;
bool light2 = false;
bool light3 = false;
bool light4 = false;
int brightness_pct = 100;
int operation_mode = 0;

float temperature_c = 28.0F;
int wifi_rssi = -100;
int uptime_minutes = 0;
String status_message = "Inicializando...";

void init_cloud_properties() {
  ArduinoCloud.setThingId(config::thing_id);

  // Placas oficiais Arduino (Uno R4 WiFi) usam autenticação por certificado (mTLS)
  // nativo no elemento seguro. setBoardId e setSecretDeviceKey só são usados em 3rd-party.
  // ArduinoCloud.setBoardId(config::device_login_name);
  // ArduinoCloud.setSecretDeviceKey(config::device_key);

  ArduinoCloud.addProperty(light1, READWRITE, ON_CHANGE, on_light1_change);
  ArduinoCloud.addProperty(light2, READWRITE, ON_CHANGE, on_light2_change);
  ArduinoCloud.addProperty(light3, READWRITE, ON_CHANGE, on_light3_change);
  ArduinoCloud.addProperty(light4, READWRITE, ON_CHANGE, on_light4_change);
  ArduinoCloud.addProperty(brightness_pct, READWRITE, ON_CHANGE, on_brightness_change);
  ArduinoCloud.addProperty(operation_mode, READWRITE, ON_CHANGE, on_operation_mode_change);

  ArduinoCloud.addProperty(temperature_c, READ, ON_CHANGE, nullptr);
  ArduinoCloud.addProperty(wifi_rssi, READ, ON_CHANGE, nullptr);
  ArduinoCloud.addProperty(uptime_minutes, READ, ON_CHANGE, nullptr);
  ArduinoCloud.addProperty(status_message, READ, ON_CHANGE, nullptr);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(config::wifi_ssid, config::wifi_pass);
