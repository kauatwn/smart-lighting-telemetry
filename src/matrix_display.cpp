/**
 * @file matrix_display.cpp
 * @brief Implementação dos efeitos e ícones visuais na matriz 12x8 onboard.
 *
 * Gerencia transições de estado visuais sem bloquear a execução principal:
 * - Busca de rede: Animação contínua de scanner Wi-Fi.
 * - Conectado: Ícone estático (Emoji Feliz).
 * - Heartbeat: Pulso periódico a cada 10s (duração de 1,5s) indicando atividade.
 *
 * Hardware: Arduino Uno R4 WiFi (Matriz de 96 LEDs vermelhos).
 */

#include "matrix_display.h"

#include <Arduino.h>
#include <Arduino_LED_Matrix.h>

namespace matrix_display {

namespace {

// Instância da biblioteca nativa da matriz de LEDs do Uno R4 WiFi
ArduinoLEDMatrix matrix;

// Controle de estado de exibição
bool cloud_connected = false;
bool heartbeat_active = false;

// Temporização não-bloqueante para o pulso de atividade (heartbeat)
constexpr unsigned long HEARTBEAT_INTERVAL_MS = 10000;  // Intervalo entre impulsos (10s)
constexpr unsigned long HEARTBEAT_DURATION_MS = 1500;   // Duração de cada pulso (1,5s)

unsigned long last_heartbeat_ms = 0;
unsigned long heartbeat_started_ms = 0;

}  // namespace

void init() {
  matrix.begin();
  show_wifi_searching();
}

void show_wifi_searching() {
  cloud_connected = false;
  heartbeat_active = false;
  matrix.loadSequence(LEDMATRIX_ANIMATION_WIFI_SEARCH);
  matrix.play(true);  // Execução em loop contínuo durante a busca
}

void show_check_success() {
  cloud_connected = true;
  heartbeat_active = false;
  last_heartbeat_ms = millis();
  matrix.loadFrame(LEDMATRIX_EMOJI_HAPPY);
}

void update(const unsigned long current_ms) {
  // Não renderiza animação de heartbeat se a placa estiver desconectada
  if (!cloud_connected) {
    return;
  }

  // Se o heartbeat estiver ativo, verifica se a duração de 1,5s expirou
  if (heartbeat_active) {
    if (current_ms - heartbeat_started_ms >= HEARTBEAT_DURATION_MS) {
      matrix.loadFrame(LEDMATRIX_EMOJI_HAPPY);
      heartbeat_active = false;
    }

    return;
  }

  // Dispara o heartbeat a cada 10s quando em operação normal
  if (current_ms - last_heartbeat_ms >= HEARTBEAT_INTERVAL_MS) {
    matrix.loadSequence(LEDMATRIX_ANIMATION_HEARTBEAT);
    matrix.play(false);  // Executa a animação de pulso apenas uma vez
    heartbeat_active = true;
    heartbeat_started_ms = current_ms;
    last_heartbeat_ms = current_ms;
  }
}

}  // namespace matrix_display
