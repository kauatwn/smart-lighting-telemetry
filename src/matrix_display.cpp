/**
 * @file matrix_display.cpp
 * @brief Implementação da exibição da matriz de LEDs.
 */

#include "matrix_display.h"

#include <Arduino.h>
#include <Arduino_LED_Matrix.h>

namespace matrix_display {
namespace {
ArduinoLEDMatrix matrix;

bool cloud_connected = false;
bool heartbeat_active = false;

constexpr unsigned long HEARTBEAT_INTERVAL_MS = 10000;
constexpr unsigned long HEARTBEAT_DURATION_MS = 1500;

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
  matrix.play(true);
}

void show_check_success() {
  cloud_connected = true;
  heartbeat_active = false;
  last_heartbeat_ms = millis();

  matrix.loadFrame(LEDMATRIX_EMOJI_HAPPY);
}

void update() {
  if (!cloud_connected) {
    return;
  }

  const unsigned long current_ms = millis();

  if (heartbeat_active) {
    if (current_ms - heartbeat_started_ms >= HEARTBEAT_DURATION_MS) {
      matrix.loadFrame(LEDMATRIX_EMOJI_HAPPY);
      heartbeat_active = false;
    }

    return;
  }

  if (current_ms - last_heartbeat_ms >= HEARTBEAT_INTERVAL_MS) {
    matrix.loadSequence(LEDMATRIX_ANIMATION_HEARTBEAT);
    matrix.play(false);

    heartbeat_active = true;
    heartbeat_started_ms = current_ms;
    last_heartbeat_ms = current_ms;
  }
}
}  // namespace matrix_display
