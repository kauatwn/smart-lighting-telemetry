/**
 * @file telemetry.h
 * @brief Módulo de coleta e cálculo de métricas de telemetria energética.
 *
 * Permite desacoplar as métricas de tempo de acionamento, quantidade de
 * acionamentos e consumo acumulado (mWh) do provedor de nuvem através de
 * injeção de ponteiros de variáveis.
 */

#pragma once

#include <stdint.h>

#include <array>

#include "lighting.h"

namespace telemetry {

/**
 * @brief Estrutura que agrupa os ponteiros para as variáveis de um canal.
 */
struct ChannelMetrics {
  float* on_time_s;        // Ponteiro para o tempo aceso acumulado (em segundos).
  float* consumption_mwh;  // Ponteiro para o consumo energético acumulado (em mWh).
  int* activations;        // Ponteiro para o contador de acionamentos.
};

/**
 * @brief Inicializa a telemetria injetando as referências dos canais.
 * @param metrics Coleção contendo as estruturas de ponteiros para os canais de iluminação.
 * @param start_ms Valor inicial de tempo obtido via millis().
 */
void init(const std::array<ChannelMetrics, lighting::CHANNEL_COUNT>& metrics, unsigned long start_ms);

/**
 * @brief Recalcula as métricas de tempo, consumo e atuações dos canais.
 * @param current_ms Tempo atual fornecido por millis().
 */
void update(unsigned long current_ms);

/**
 * @brief Zera todas as métricas acumuladas nos ponteiros vinculados.
 */
void reset();

}  // namespace telemetry
