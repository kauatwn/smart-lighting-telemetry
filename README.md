# Sistema de Iluminação Inteligente com Alexa

> **Projeto de Automação Residencial e IoT (Arduino Uno R4 WiFi)**  
> _Controle de iluminação multicanal via comando de voz (Amazon Alexa), aplicativo e navegador, com feedback visual na matriz de LEDs integrada, acionamento por relé e telemetria em tempo real._

## Sobre o Projeto

Este projeto permite controlar a iluminação de até 4 cômodos através de comandos de voz com a **Amazon Alexa**, aplicativo no celular ou painel web pelo **Arduino IoT Cloud**. O sistema suporta ajuste de brilho (dimmer de 0 a 100%), efeitos luminosos, comutação de potência via **módulo relé** para controle de cargas reais, feedback visual por animações na matriz de LEDs integrada e monitoramento contínuo da temperatura interna do circuito.

## 1. Conexões de Hardware (Pinout)

O circuito foi desenvolvido para o **Arduino Uno R4 WiFi**, utilizando saídas PWM para controle proporcional e acionamento de relé:

| Componente                | Pino Uno R4 | Tipo de I/O          | Função no Sistema          | Detalhes da Montagem Elétrica                   |
|:--------------------------|:------------|:---------------------|:---------------------------|:------------------------------------------------|
| **LED 1 (Cômodo 1)**      | `D3 (~)`    | Saída PWM            | Iluminação Zona 1          | Ânodo no D3; Cátodo no resistor de 220 Ω ao GND |
| **LED 2 (Cômodo 2)**      | `D5 (~)`    | Saída PWM            | Iluminação Zona 2          | Ânodo no D5; Cátodo no resistor de 220 Ω ao GND |
| **LED 3 (Cômodo 3)**      | `D6 (~)`    | Saída PWM            | Iluminação Zona 3          | Ânodo no D6; Cátodo no resistor de 220 Ω ao GND |
| **LED 4 (Cômodo 4)**      | `D9 (~)`    | Saída PWM            | Iluminação Zona 4          | Ânodo no D9; Cátodo no resistor de 220 Ω ao GND |
| **Módulo Relé (5V)**      | `D3 (~)`    | Saída Digital/PWM    | Comutação de carga real    | VCC em 5V, GND em GND e Sinal (IN) no pino D3   |
| **Matriz de LEDs (12x8)** | `Onboard`   | Barramento Interno   | Feedback visual do nó      | Integrada na placa (96 LEDs vermelhos)          |
| **Sensor Térmico do MCU** | `Interno`   | ADC Renesas (14-bit) | Temperatura do processador | Registradores de silício `R_ADC0` e `R_MSTP`    |
| **Rádio Wi-Fi / MQTT**    | `ESP32-S3`  | Barramento SPI/UART  | Comunicação segura mTLS    | Coprocessador de conectividade integrado        |

> [!TIP]
> **Resistores Limitadores e Relé:** Os resistores de 220 Ω no terminal catódico protegem os pinos do microcontrolador (limite de 8 mA por pino). O módulo relé de 5V permite comutar circuitos de potência reais com isolamento óptico, atendendo à demonstração prática de acionamento sem necessidade de lâmpadas de alta tensão na bancada.

## 2. Modos de Operação

| Modo                  | Descrição                      | Comportamento dos LEDs                        | Matriz de LEDs (`gallery.h`)                                    |
|:----------------------|:-------------------------------|:----------------------------------------------|:----------------------------------------------------------------|
| **0 - Manual**        | Controle individual por cômodo | Respeita o brilho definido (0 a 100%)         | `LEDMATRIX_EMOJI_HAPPY` ($\ge 1$) / `LEDMATRIX_EMOJI_SAD` ($0$) |
| **1 - Todas Ligadas** | Acende todos os canais juntos  | Liga todas as luzes simultaneamente           | `LEDMATRIX_EMOJI_HAPPY`                                         |
| **2 - Respiração**    | Efeito suave de luz pulsante   | Variação gradual de brilho sem travar a placa | `LEDMATRIX_EMOJI_HAPPY`                                         |
| **3 - Alerta**        | Alerta visual de emergência    | Pisca rápido em 5 Hz                          | `LEDMATRIX_DANGER`                                              |

> [!TIP]
> Ao ajustar o brilho no app ou por voz, a matriz de LEDs exibe temporariamente a animação de nível de bateria (`LEDMATRIX_ANIMATION_BATTERY`) como confirmação visual.

## 3. Variáveis Sincronizadas no Arduino IoT Cloud

Configure as seguintes propriedades no seu **Thing** no Arduino Cloud para espelhar as variáveis do firmware:

| Variável Cloud   | Tipo de Dado | Permissão    | Política de Atualização | Descrição / Função no Dashboard e Alexa                      |
|:-----------------|:-------------|:-------------|:------------------------|:-------------------------------------------------------------|
| `light1`         | `Boolean`    | Read & Write | `ON_CHANGE`             | Interruptor liga/desliga da Zona 1 / Relé                    |
| `light2`         | `Boolean`    | Read & Write | `ON_CHANGE`             | Interruptor liga/desliga da Zona 2                           |
| `light3`         | `Boolean`    | Read & Write | `ON_CHANGE`             | Interruptor liga/desliga da Zona 3                           |
| `light4`         | `Boolean`    | Read & Write | `ON_CHANGE`             | Interruptor liga/desliga da Zona 4                           |
| `brightness_pct` | `Integer`    | Read & Write | `ON_CHANGE`             | Slider de ajuste do dimmer geral (0–100%)                    |
| `operation_mode` | `Integer`    | Read & Write | `ON_CHANGE`             | Seletor de modo (0: Manual, 1: All On, 2: Breathe, 3: Alert) |
| `temperature_c`  | `Float`      | Read Only    | `ON_CHANGE`             | Gráfico temporal da temperatura do processador (°C)          |
| `wifi_rssi`      | `Integer`    | Read Only    | `ON_CHANGE`             | Medidor de intensidade do sinal Wi-Fi (dBm)                  |
| `uptime_minutes` | `Integer`    | Read Only    | `ON_CHANGE`             | Tempo de atividade ininterrupto da placa (minutos)           |
| `status_message` | `String`     | Read Only    | `ON_CHANGE`             | Mensagem de diagnóstico do nó (_Normal_, _Alerta_)           |

## 4. Como Executar

1. Clone o repositório e entre na pasta:

   ```bash
   git clone https://github.com/kauatwn/smart-lighting-telemetry.git
   cd smart-lighting-telemetry
   ```

2. Crie seu arquivo de credenciais a partir do modelo (`cp include/secrets.example.h include/secrets.h`) e preencha seu Wi-Fi e `thing_id`.
3. Compile e grave na placa conectada via USB:

   ```bash
   pio run -t upload
   ```

4. Para controle por voz, ative a Skill **Arduino** no aplicativo da **Amazon Alexa** e faça login com sua conta do Arduino Cloud.
