# Prótese Robótica de Mão

<p align="center">
  <img src="https://skillicons.dev/icons?i=vscode,cpp" />
</p>

Mão robótica impressa em 3D, controlada por ESP32 e servomotores, desenvolvida para o projeto das disciplinas PIC SW e PIC HW.

![Imagem da Prótese](https://via.placeholder.com/600x400?text=Imagem+da+Prótese+Robótica+de+Mão)

## Índice

- [Visão Geral](#visão-geral)
- [Requisitos de Hardware](#requisitos-de-hardware)
- [Bibliotecas Necessárias](#bibliotecas-necessárias)
- [Configuração do Ambiente](#configuração-do-ambiente)
  - [Instalação da Arduino IDE](#instalação-da-arduino-ide)
  - [Configuração do ESP32](#configuração-do-esp32)
- [Instalação e Configuração do Projeto](#instalação-e-configuração-do-projeto)
- [Estrutura do Projeto](#estrutura-do-projeto)
- [Utilização](#utilização)
- [Solução de Problemas](#solução-de-problemas)
- [Contribuições](#contribuições)
- [Licença](#licença)

## Visão Geral

Este projeto consiste em uma prótese robótica de mão controlada por um ESP32, utilizando servomotores para movimentar os dedos individualmente. A comunicação com a prótese é feita via Bluetooth LE, permitindo controle remoto dos movimentos de cada dedo.

## Requisitos de Hardware

- ESP32 (DevKit ou similar)
- 5 Servomotores (compatíveis com ESP32)
- Impressora 3D (para imprimir as peças da mão)
- Cabos jumper
- Fonte de alimentação para os servomotores (5V)
- Bateria ou fonte de energia portátil (opcional, para uso móvel)

## Bibliotecas Necessárias

Para executar este projeto, você precisará das seguintes bibliotecas:

| Biblioteca | Versão | Descrição |
|------------|--------|-----------|
| ESP32Servo | 0.9.0 ou superior | Controle dos servomotores |
| BLEDevice | Incluída no ESP32 | Comunicação Bluetooth LE |
| ArduinoJson | 6.x ou superior | Processamento de comandos JSON |

## Configuração do Ambiente

### Instalação da Arduino IDE

1. Baixe e instale a [Arduino IDE](https://www.arduino.cc/en/software) (versão 2.0 ou superior recomendada).
2. Abra a Arduino IDE.

### Configuração do ESP32

1. Abra a Arduino IDE.
2. Vá para "Arquivo > Preferências".
3. No campo "URLs Adicionais para Gerenciadores de Placas", adicione:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Clique em "OK".
5. Vá para "Ferramentas > Placa > Gerenciador de Placas".
6. Pesquise por "ESP32" e instale o pacote "ESP32 by Espressif Systems".
7. Selecione sua placa ESP32:
   - "Ferramentas > Placa > ESP32 Arduino > ESP32 Dev Module" (ou o modelo específico da sua placa)
   - "Ferramentas > Porta > [Porta COM do seu ESP32]"

## Instalação e Configuração do Projeto

1. Clone este repositório:
   ```bash
   git clone https://github.com/jcquadros/controlador-protese-robotica-de-mao.git
   ```

2. Instale as bibliotecas necessárias:
   - Na Arduino IDE, vá para "Ferramentas > Gerenciar Bibliotecas".
   - Pesquise e instale:
     - ESP32Servo
     - ArduinoJson

3. Conecte o ESP32 ao seu computador via USB.

4. Abra o arquivo `protese-robotica-de-mao.ino` na Arduino IDE.

5. Verifique as configurações dos pinos dos servomotores no arquivo e ajuste conforme necessário.

6. Compile e faça o upload do código para o ESP32 clicando no botão "Carregar" (seta para a direita).

## Estrutura do Projeto

- **protese-robotica-de-mao.ino**: Arquivo principal do projeto, contém as configurações iniciais e loop principal.
- **bluetooth-service.h/cpp**: Implementação do serviço Bluetooth LE.
- **hand.h/cpp**: Controle da mão robótica como um todo.
- **finger.h/cpp**: Classe para controle individual de cada dedo.
- **hand-command.h/cpp**: Estrutura de comandos para controlar a mão.

## Utilização

1. Após fazer o upload do código para o ESP32, a prótese estará pronta para uso.
2. O dispositivo Bluetooth será anunciado como "Bucky Barnes Arm".
3. Conecte-se ao dispositivo usando um aplicativo BLE capaz de enviar comandos JSON.
4. Envie comandos JSON no formato:
   ```json
   {
     "thumb": 50,
     "index": 75,
     "middle": 100,
     "ring": 25,
     "pinky": 0
   }
   ```
   Onde cada valor representa a porcentagem de fechamento do dedo (0-100%).

## Solução de Problemas

- **Servomotores não se movem**: Verifique a alimentação dos servomotores. Eles podem precisar de uma fonte externa de 5V.
- **ESP32 não é detectado**: Verifique a conexão USB e instale os drivers necessários.
- **Erro na compilação**: Verifique se todas as bibliotecas foram instaladas corretamente.
- **Problemas de conexão Bluetooth**: Reinicie o ESP32 e tente conectar novamente.