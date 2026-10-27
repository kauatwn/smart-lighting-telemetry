# Sistema de Iluminação Residencial Inteligente com Alexa e Telemetria

> **Projeto de Automação Residencial e IoT (Arduino Uno R4 WiFi)**  
> _Controle de iluminação por cômodo via Amazon Alexa, aplicativo e web, acionamento híbrido por relé e botão físico não-bloqueante, feedback visual na matriz de LEDs 12x8 e telemetria de consumo de energia._

## Sobre o Projeto

Este projeto permite controlar a iluminação de 4 cômodos (**Cozinha**, **Sala**, **Banheiro** e **Varanda**) através de comandos de voz com a **Amazon Alexa**, aplicativo no celular ou painel web pelo **Arduino IoT Cloud**.

O sistema possui acionamento híbrido para a **Cozinha**, integrando o LED do ambiente e um **Módulo Relé de 5V** para comutação de carga real, que pode ser alternado remotamente ou por um **botão físico** com filtro de _debounce_ não-bloqueante (`millis()`). A matriz de LEDs 12x8 integrada exibe animações de status da rede e pulso de atividade (_heartbeat_), enquanto o subsistema de telemetria calcula em tempo real o tempo aceso, o número de acionamentos e a estimativa de consumo de energia em mWh para cada ambiente.

## 1. Conexões de Hardware (Pinout)

O circuito foi desenvolvido para o **Arduino Uno R4 WiFi**, utilizando saídas digitais para os LEDs e relé, e entrada com resistor interno de _pull-up_ para o botão físico:

| Componente                | Pino Uno R4 | Tipo de I/O              | Função no Sistema                 | Detalhes da Montagem Elétrica                                               |
|:--------------------------|:------------|:-------------------------|:----------------------------------|:----------------------------------------------------------------------------|
| **LED Sala**              | `D3`        | Saída Digital            | Iluminação da Sala                | Ânodo no D3; Cátodo no resistor de 220 Ω ao GND                             |
| **LED Banheiro**          | `D4`        | Saída Digital            | Iluminação do Banheiro            | Ânodo no D4; Cátodo no resistor de 220 Ω ao GND                             |
| **LED Varanda**           | `D5`        | Saída Digital            | Iluminação da Varanda             | Ânodo no D5; Cátodo no resistor de 220 Ω ao GND                             |
| **LED Cozinha**           | `D6`        | Saída Digital            | Iluminação da Cozinha             | Ânodo no D6; Cátodo no resistor de 220 Ω ao GND                             |
| **Módulo Relé (5V)**      | `D9`        | Saída Digital            | Comutação de Carga Real (Cozinha) | VCC em 5V, GND em GND e Sinal (IN) no D9 (Lógica Invertida: `LOW` = LIGADO) |
| **Botão Físico**          | `D12`       | Entrada (`INPUT_PULLUP`) | Comutação Local da Cozinha / Relé | Um terminal no D12, outro no GND (_Debounce_ de 50 ms via `millis()`)       |
| **Matriz de LEDs (12x8)** | `Onboard`   | Barramento Interno       | Feedback Visual de Status         | Integrada na placa (96 LEDs vermelhos)                                      |
| **Rádio Wi-Fi / MQTT**    | `ESP32-S3`  | Barramento SPI/UART      | Comunicação Cloud / Alexa         | Coprocessador de conectividade integrado                                    |

> [!TIP]
> **Relé e Botão Físico:** O módulo relé funciona em lógica invertida (`LOW` para acionar) e é chaveado simultaneamente com o LED da Cozinha. O botão físico no pino D12 alterna o estado da Cozinha localmente e sincroniza o novo estado com a Nuvem e a Alexa em tempo real.

## 2. Comportamento da Matriz de LEDs

A matriz de 12x8 LEDs integrada fornece diagnósticos visuais não-bloqueantes do estado da placa e da conexão:

| Estado do Sistema             | Animação / Ícone                  | Função no Display                                                    |
|:------------------------------|:----------------------------------|:---------------------------------------------------------------------|
| **Conectando ao Wi-Fi**       | `LEDMATRIX_ANIMATION_WIFI_SEARCH` | Animação contínua de busca de rede / Nuvem                           |
| **Conectado / Normal**        | `LEDMATRIX_EMOJI_HAPPY`           | Exibe o ícone "Feliz" confirmando operação normal                    |
| **Pulso do Loop (Heartbeat)** | `LEDMATRIX_ANIMATION_HEARTBEAT`   | Disparado a cada 10 s (duração de 1,5 s) indicando execução contínua |

## 3. Variáveis Sincronizadas no Arduino IoT Cloud

Configure as seguintes propriedades no seu **Thing** no Arduino Cloud para espelhar as variáveis do firmware e permitir a integração automática com a **Amazon Alexa**:

### Controles Remotos (Atuação)

| Variável Cloud | Tipo de Dado  | Permissão    | Política de Atualização | Descrição / Função no Dashboard e Alexa                |
|:---------------|:--------------|:-------------|:------------------------|:-------------------------------------------------------|
| `cozinha`      | `CloudSwitch` | Read & Write | `ON_CHANGE`             | Interruptor liga/desliga da Cozinha (LED D6 + Relé D9) |
| `sala`         | `CloudSwitch` | Read & Write | `ON_CHANGE`             | Interruptor liga/desliga da Sala (LED D3)              |
| `banheiro`     | `CloudSwitch` | Read & Write | `ON_CHANGE`             | Interruptor liga/desliga do Banheiro (LED D4)          |
| `varanda`      | `CloudSwitch` | Read & Write | `ON_CHANGE`             | Interruptor liga/desliga da Varanda (LED D5)           |

### Métricas de Telemetria (Somente Leitura)

| Variável Cloud             | Tipo de Dado | Permissão | Política de Atualização | Descrição / Função no Dashboard                            |
|:---------------------------|:-------------|:----------|:------------------------|:-----------------------------------------------------------|
| `[comodo]_on_time_s`       | `Float`      | Read Only | Every 5 Seconds         | Tempo acumulado do cômodo aceso (em segundos)              |
| `[comodo]_activations`     | `Integer`    | Read Only | `ON_CHANGE`             | Contador total de acionamentos do cômodo                   |
| `[comodo]_consumption_mwh` | `Float`      | Read Only | Every 5 Seconds         | Consumo estimado do cômodo em mWh (potência nominal de 9W) |
| `wifi_rssi_dbm`            | `Integer`    | Read Only | Every 5 Seconds         | Intensidade do sinal Wi-Fi recebido (dBm)                  |

> [!NOTE]
> Substitua `[comodo]` pelas propriedades correspondentes de cada ambiente (`cozinha`, `sala`, `banheiro`, `varanda`).
