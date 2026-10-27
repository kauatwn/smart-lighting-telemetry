/**
 * @file telemetry.h
 * @brief Interface de coleta de métricas operacionais.
 *
 * Registra o tempo ligado, a quantidade de acionamentos e a
 * energia estimada de cada ponto de iluminação. Também monitora
 * a intensidade do sinal Wi-Fi.
 */

#pragma once

namespace telemetry {
/**
 * @brief Inicializa o acompanhamento das métricas.
 * @param start_ms Valor inicial de millis().
 *
 * Zera as métricas e registra os estados iniciais da iluminação.
 */
void init(unsigned long start_ms);

/**
 * @brief Atualiza as métricas operacionais.
 * @param current_ms Valor atual de millis().
 *
 * Acumula o tempo ligado e a energia estimada, identifica novos
 * acionamentos e atualiza periodicamente o sinal Wi-Fi.
 */
void update(unsigned long current_ms);
}  // namespace telemetry
