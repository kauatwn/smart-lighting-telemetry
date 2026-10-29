/**
 * @file matrix_display.h
 * @brief Interface de controle visual para a matriz de LEDs 12x8 integrada.
 *
 * Exibe diagnósticos visuais não-bloqueantes do estado de conexão Wi-Fi/Nuvem
 * e pulso de atividade (heartbeat) no hardware do Arduino Uno R4 WiFi.
 */

#pragma once

namespace matrix_display {

/**
 * @brief Inicializa a matriz de LEDs integrada e ativa a animação de busca por rede.
 */
void init();

/**
 * @brief Atualiza a temporização e o ciclo de animação do heartbeat na matriz.
 * @param current_ms Tempo atual do sistema fornecido por millis().
 */
void update(unsigned long current_ms);

/**
 * @brief Exibe a animação contínua de busca por conexão Wi-Fi e Nuvem.
 */
void show_wifi_searching();

/**
 * @brief Exibe o ícone de confirmação (Emoji Feliz) indicando conexão bem-sucedida.
 */
void show_check_success();

}  // namespace matrix_display
