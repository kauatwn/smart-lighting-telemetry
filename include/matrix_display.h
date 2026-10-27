/**
 * @file matrix_display.h
 * @brief Interface de exibição da matriz de LEDs integrada.
 */

#pragma once

namespace matrix_display {
/**
 * @brief Inicializa a matriz de LEDs.
 */
void init();

/**
 * @brief Atualiza a animação periódica de coração.
 */
void update();

/**
 * @brief Exibe a animação de busca por conexão.
 */
void show_wifi_searching();

/**
 * @brief Exibe o rosto feliz após conectar à Cloud.
 */
void show_check_success();
}  // namespace matrix_display
