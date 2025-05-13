![image](https://github.com/user-attachments/assets/f2a5c9b8-6208-4723-8f46-1d74be421827)


# 🌡️ Projeto: Sistema de Monitoramento Ambiental com Raspberry Pi Pico W

## 📑 Sumário
- [🎯 Objetivos](#-objetivos)
- [📋 Descrição do Projeto](#-descrição-do-projeto)
- [⚙️ Funcionalidades Implementadas](#️-funcionalidades-implementadas)
- [🛠️ Requisitos do Projeto](#️-requisitos-do-projeto)
- [📂 Estrutura do Repositório](#-estrutura-do-repositório)
- [🖥️ Como Compilar](#️-como-compilar)
- [🤝 Contribuições](#-contribuições)
- [📽️ Demonstração em Vídeo](#️-demonstração-em-vídeo)
- [💡 Considerações Finais](#-considerações-finais)

## 🎯 Objetivos
- Desenvolver um sistema de monitoramento ambiental utilizando o Raspberry Pi Pico W
- Implementar comunicação sem fio via WiFi para controle remoto
- Apresentar dados de temperatura e umidade via interface web e display OLED
- Criar um sistema de alerta visual e sonoro para condições ambientais críticas

## 📋 Descrição do Projeto
Este projeto utiliza o Raspberry Pi Pico W com os seguintes componentes:
- Display OLED SSD1306 via I2C (GPIO 14 e 15)
- LEDs RGB (GPIO 11, 12, 13)
- Joystick analógico (GPIO 26 e 27)
- Botões (GPIO 5 e 6)
- Matriz de LED WS2812 (GPIO 7)
- Buzzer (GPIO 10 e 21)
- Conexão WiFi integrada

## ⚙️ Funcionalidades Implementadas
1. **Servidor Web:**
   - Interface web para visualização de temperatura e umidade
   - Controle remoto de dispositivos (ventilador, aquecedor, umidificador, desumidificador)
   - Visualização de alertas no navegador com indicação colorida de status

2. **Sistema de Alerta:**
   - Notificações visuais através da matriz de LEDs WS2812
   - Alertas sonoros via buzzer para condições críticas
   - Diferentes padrões visuais na matriz de LEDs para cada condição ambiental:
     - Temperatura elevada: Chama em vermelho
     - Temperatura baixa: Floco de neve em azul
     - Umidade elevada: Seta para cima em ciano
     - Umidade baixa: Seta para baixo em laranja

3. **Display OLED:**
   - Exibição em tempo real dos valores de temperatura e umidade
   - Interface visual com divisão em quadrantes para organização das informações
   - Atualização contínua dos dados

4. **Controle Analógico:**
   - Ajuste de valores de temperatura e umidade via joystick
   - Interface intuitiva para simulação de condições ambientais
   - Deadzone configurada para evitar leituras acidentais

5. **Controle Manual:**
   - Botões físicos para ligar/desligar ventilador e umidificador
   - Indicação por LEDs do status dos dispositivos
     
## 🛠️ Requisitos do Projeto
- **Uso de WiFi:** Acesso remoto através de servidor web
- **Comunicação I2C:** Integração com display OLED SSD1306
- **Manipulação de PWM:** Controle de buzzer e LEDs
- **Protocolo PIO:** Controle da matriz de LEDs WS2812
- **Conversão ADC:** Leitura de valores analógicos do joystick
- **Interrupções GPIO:** Detecção de pressão de botões
- **TCP/IP Stack:** Implementação de servidor web usando lwIP
- **Organização do Código:** Estrutura modular e bem comentada

## 📂 Estrutura do Repositório
```
├── Projeto_IOT.c              # Código principal do projeto
├── lwipopts.h          # Biblioteca para o wifi
├── lwipopts_exemples_common.h # Biblioteca auxiliar para o wifi
├── ws2812.pio          # Programa PIO para controle dos LEDs WS2812
├── ssd1306.c           # Biblioteca para controle do display OLED
├── ssd1306.h           # Cabeçalho da biblioteca do display
├── font.h              # Definições de fonte para o display
└── CMakeLists.txt      # Arquivo de configuração de compilação
```

## 🖥️ Como Compilar
1. Clone o repositório:
   ```bash
   git clone https://github.com/JPSRaccolto/projeto_ambiental.git
   ```
2. Navegue até o diretório do projeto:
   ```bash
   cd projeto_ambiental
   ```
3. Compile o projeto com seu ambiente de desenvolvimento configurado para o RP2040.
4. Carregue o código na placa BitDogLab.

## 🧑‍💻 Autor
**João Pedro Soares Raccolto**

## 📝 Descrição
Sistema de monitoramento ambiental utilizando Raspberry Pi Pico W que permite o acompanhamento de temperatura e umidade através de uma interface web e um display OLED. O sistema inclui alertas sonoros e visuais para condições ambientais críticas e permite controle remoto de dispositivos ambientais.

## 🤝 Contribuições
Contribuições são bem-vindas! Siga os passos abaixo para contribuir:

1. Fork este repositório.
2. Crie uma nova branch:
   ```bash
   git checkout -b minha-feature
   ```
3. Faça suas modificações e commit:
   ```bash
   git commit -m 'Adicionada nova funcionalidade'
   ```
4. Envie suas alterações:
   ```bash
   git push origin minha-feature
   ```
5. Abra um Pull Request.

## 📽️ Demonstração em Vídeo
- O vídeo de demonstração do projeto pode ser visualizado aqui: [Link para o vídeo](https://drive.google.com/file/d/1CtTBlMcizYix0AwHNDmm700G4hOCUKI9/view?usp=sharing)

## 💡 Considerações Finais
Este projeto demonstra a versatilidade da Raspberry Pi Pico W em aplicações de IoT e monitoramento ambiental. O sistema fornece uma solução completa para acompanhamento de temperatura e umidade, com alertas e controle remoto, sendo ideal para casas inteligentes, estufas, salas de servidores ou qualquer ambiente que necessita de monitoramento constante das condições climáticas.
A integração entre os elementos de interface (display OLED e servidor web) com os sistemas de alertas visuais e sonoros cria uma solução robusta e completa para monitoramento ambiental. O código foi estruturado de forma modular para facilitar futuras expansões e adaptações do sistema.
