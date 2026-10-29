/**
 * @file lighting.h
 * @brief Interface de controle das saídas digitais de iluminação e periféricos físicos.
 *
 * Concentra a inicialização e o acionamento dos 4 canais de LED, do módulo relé
 * e o tratamento não-bloqueante do botão físico com debounce via millis().
 */

#pragma once

#include <stdint.h>

namespace lighting {

/**
 * @brief Identificadores dos canais de iluminação do sistema.
 */
enum class Channel : uint8_t {
  COZINHA = 0,
  SALA = 1,
  BANHEIRO = 2,
  VARANDA = 3,
};

// Quantidade total de canais de iluminação controlados pela placa.
constexpr uint8_t CHANNEL_COUNT = 4;

/**
 * @brief Configura a direção dos pinos de hardware e garante o estado inicial desligado.
 */
void init();

/**
 * @brief Altera o estado lógico de um canal de iluminação.
 * @param channel Canal a ser acionado (ex: Channel::COZINHA).
 * @param enabled true para ligar (HIGH); false para desligar (LOW).
 */
void set_channel(Channel channel, bool enabled);

/**
 * @brief Consulta o estado lógico atual de um canal.
 * @param channel Canal a ser consultado.
 * @return true se o canal estiver aceso; false caso contrário.
 */
bool get_channel(Channel channel);

/**
 * @brief Altera o estado do relé da Cozinha.
 * @param enabled true para acionar o relé (LOW); false para desativar (HIGH).
 */
void set_relay(bool enabled);

/**
 * @brief Consulta o estado lógico atual do relé.
 * @return true se o relé estiver acionado; false caso contrário.
 */
bool get_relay_state();

/**
 * @brief Processa o estado do botão físico aplicando filtro de debounce temporal.
 * @param current_ms Tempo atual do sistema obtido via millis().
 * @return true se o botão foi pressionado e alternou o estado do relé nesta chamada.
 */
bool process_physical_button(unsigned long current_ms);

}  // namespace lighting
