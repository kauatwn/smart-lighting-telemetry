/**
 * @file lighting.h
 * @brief Interface de controle das saídas de iluminação.
 *
 * Centraliza o controle dos quatro LEDs, do relé e do botão
 * físico. A integração com a Arduino IoT Cloud é realizada
 * por outro módulo.
 */

#pragma once

#include <stdint.h>

namespace lighting {
/**
 * @brief Identificadores legíveis para cada canal de iluminação.
 */
enum class Channel : uint8_t {
  COZINHA = 0,
  SALA = 1,
  BANHEIRO = 2,
  VARANDA = 3,
};

/**
 * @brief Quantidade de pontos de iluminação representados por LEDs.
 */
constexpr uint8_t CHANNEL_COUNT = 4;

/**
 * @brief Inicializa as saídas e o botão físico.
 *
 * As saídas começam desligadas. O relé utiliza lógica invertida:
 * LOW aciona o relé e HIGH o mantém desligado.
 */
void init();

/**
 * @brief Atualiza o estado de um ponto de iluminação.
 * @param channel Canal de iluminação (ex: lighting::COZINHA).
 * @param enabled true para ligar; false para desligar.
 */
void set_channel(Channel channel, bool enabled);

/**
 * @brief Consulta o estado lógico de um ponto de iluminação.
 * @param channel Canal de iluminação (ex: lighting::COZINHA).
 * @return true se o ponto estiver ligado; false caso contrário.
 */
bool get_channel(Channel channel);

/**
 * @brief Atualiza o estado do relé.
 * @param enabled true para acionar; false para desligar.
 */
void set_relay(bool enabled);

/**
 * @brief Consulta o estado lógico do relé.
 */
bool get_relay_state();

/**
 * @brief Processa o botão físico sem bloquear o loop principal.
 * @param current_ms Valor atual de millis().
 * @return true se o botão alternou o estado do relé; false caso contrário.
 */
bool process_physical_button(unsigned long current_ms);
}  // namespace lighting
