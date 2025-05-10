#include <stdio.h>               // Biblioteca padrão para entrada e saída
#include <string.h>              // Biblioteca manipular strings
#include <stdlib.h>              // funções para realizar várias operações, incluindo alocação de memória dinâmica (malloc)

#include "pico/stdlib.h"         // Biblioteca da Raspberry Pi Pico para funções padrão (GPIO, temporização, etc.)
#include "hardware/pwm.h"
#include "hardware/adc.h"        // Biblioteca da Raspberry Pi Pico para manipulação do conversor ADC
#include "pico/cyw43_arch.h"     // Biblioteca para arquitetura Wi-Fi da Pico com CYW43  
#include "lwip/pbuf.h"           // Lightweight IP stack - manipulação de buffers de pacotes de rede
#include "lwip/tcp.h"            // Lightweight IP stack - fornece funções e estruturas para trabalhar com o protocolo TCP
#include "lwip/netif.h"          // Lightweight IP stack - fornece funções e estruturas para trabalhar com interfaces de rede (netif)
#include "ws2812.pio.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "font.h"

// Credenciais WIFI - Tome cuidado se publicar no github!
#define WIFI_SSID "Galaxy S20 FE 5G"
#define WIFI_PASSWORD "abcd9700"

// Definição dos pinos dos LEDs
#define LED_PIN CYW43_WL_GPIO_LED_PIN   // GPIO do CI CYW43
#define BOTAO_A 5
#define AZUL 12
#define VERDE 11
#define VERMELHO 13
#define VERTICALY 26
#define HORIZONTALX 27
#define SELEC 22
#define PWM_WRAP 4095
#define PWM_CLK_DIV 30.52f
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ENDERECO_I2C 0x3C
#define buzzer1 10
#define buzzer2 21
#define IS_RGBW false 
#define NUM_PIXELS 25
#define WS2812_PIN 7
#define DEADZONE 170
ssd1306_t ssd;
absolute_time_t last_interrupt_time = 0;
bool global =  false;
bool global1 = false;
bool global2 = false;
bool global3 = false;
float temperatura = 25.0f;
float umidade = 50.0f;

static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

int i = 0;

double desenho0[25] = {
    0.2, 0.0, 0.2, 0.0, 0.2,
    0.0, 0.2, 0.2, 0.2, 0.0,
    0.2, 0.2, 0.2, 0.2, 0.2,
    0.0, 0.2, 0.2, 0.2, 0.0,
    0.2, 0.0, 0.2, 0.0, 0.2
};

void num0(uint8_t r, uint8_t g, uint8_t b){
    
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);  
     // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (desenho0[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
    
}

// Desenho do número 1
double desenho1[25] = 
    {0.2, 0.2, 0.2, 0.2, 0.2,
     0.2, 0.2, 0.2, 0.2, 0.2,
     0.2, 0.2, 0.2, 0.0, 0.2,
     0.2, 0.0, 0.2, 0.0, 0.2,
     0.2, 0.0, 0.0, 0.0, 0.2};

void num1(uint8_t r, uint8_t g, uint8_t b){
    
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);  
     // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (desenho1[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
    
}
// Desenho do número 2
double desenho2[25] = 
    {0.0, 0.0, 0.2, 0.0, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0,
     0.2, 0.0, 0.2, 0.0, 0.2,
     0.0, 0.0, 0.2, 0.0, 0.0,
     0.0, 0.0, 0.2, 0.0, 0.0};

void num2(uint8_t r, uint8_t g, uint8_t b){
    
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);  
     // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (desenho2[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
    
}
// Desenho do número 3
double desenho3[25] = 
    {0.0, 0.0, 0.2, 0.0, 0.0,
     0.0, 0.0, 0.2, 0.0, 0.0,
     0.2, 0.0, 0.2, 0.0, 0.2,
     0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.0, 0.2, 0.0, 0.0};

void num3(uint8_t r, uint8_t g, uint8_t b){
    
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);  
     // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (desenho3[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
    
}
// Função de callback ao aceitar conexões TCP
static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err);

// Função de callback para processar requisições HTTP
static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);

// Tratamento do request do usuário
void user_request(char **request);

//configurações do PWM
void pwm_init_gpio(uint gpio, uint wrap, float clkdiv) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_wrap(&config, wrap);
    pwm_config_set_clkdiv(&config, clkdiv);
    pwm_init(slice_num, &config, true);
}


void adc_incremental(float *temperatura, float *umidade) {
    adc_select_input(0); // eixo Y
    uint16_t valor_y = adc_read();
    adc_select_input(1); // eixo X
    uint16_t valor_x = adc_read();

    int16_t diff_y = 2048 - (int16_t)valor_y ;
    int16_t diff_x = (int16_t)valor_x - 2048;

    if (abs(diff_y) > DEADZONE) {
        *temperatura += (diff_y > 0) ? -2.0f : 2.0f;
        if (*temperatura < -20.0f) *temperatura = -20.0f;
        if (*temperatura > 100.0f) *temperatura = 100.0f;
    }

    if (abs(diff_x) > DEADZONE) {
        *umidade += (diff_x > 0) ? 2.0f : -2.0f;
        if (*umidade < 0.0f) *umidade = 0.0f;
        if (*umidade > 100.0f) *umidade = 100.0f;
    }
}

void analise(){
    if(temperatura > 55.0f){
        //global = !global;
        printf("Temperatura elevada, por favor ligue o ventilador\n");
        num1(255,0,0);
        pwm_set_gpio_level(buzzer1, 250); // 50%
        pwm_set_gpio_level(buzzer2, 250);
        sleep_ms(1000);
        pwm_set_gpio_level(buzzer1, 0);   // silencia
        pwm_set_gpio_level(buzzer2, 0);   // silencia
        sleep_ms(1000);
        //colocar o liga/desliga no webserver
    }
    else if(temperatura < 5.0f){
        //global1 = !global1;
        printf("Sistema operando em temperaturas críticas, ligue o aquecedor\n");
        num0(0,0,255);
        pwm_set_gpio_level(buzzer1, 250); // 50%
        pwm_set_gpio_level(buzzer2, 250);
        sleep_ms(400);
        pwm_set_gpio_level(buzzer1, 0);   // silencia
        pwm_set_gpio_level(buzzer2, 0);   // silencia
        sleep_ms(400);
        pwm_set_gpio_level(buzzer1, 250); // 50%
        pwm_set_gpio_level(buzzer2, 250);
        sleep_ms(400);
        pwm_set_gpio_level(buzzer1, 0);   // silencia
        pwm_set_gpio_level(buzzer2, 0);   // silencia
        sleep_ms(400);

    }
    else if(umidade > 70.0f){
        //global2 = !global2;
        printf("Umidade muitoo elevada, ligue o desumidificador\n");
        num3(0,255,255);
        pwm_set_gpio_level(buzzer1, 250); // 50%
        pwm_set_gpio_level(buzzer2, 250);
        sleep_ms(700);
        pwm_set_gpio_level(buzzer1, 0);   // silencia
        pwm_set_gpio_level(buzzer2, 0);   // silencia
        sleep_ms(700);
        pwm_set_gpio_level(buzzer1, 250); // 50%
        pwm_set_gpio_level(buzzer2, 250);
        sleep_ms(700);
        pwm_set_gpio_level(buzzer1, 0);   // silencia
        pwm_set_gpio_level(buzzer2, 0);   // silencia
        sleep_ms(000);
        pwm_set_gpio_level(buzzer1, 250); // 50%
        pwm_set_gpio_level(buzzer2, 250);
        sleep_ms(700);
        pwm_set_gpio_level(buzzer1, 0);   // silencia
        pwm_set_gpio_level(buzzer2, 0);   // silencia
        sleep_ms(700);

    }
    else if(umidade < 30.0f){
        //global3 = !global3;
        printf("Umidade em situação crítica, ligue o umidificador\n");
        num2(255,165,0);
        pwm_set_gpio_level(buzzer1, 250); // 50%
        pwm_set_gpio_level(buzzer2, 250);
        sleep_ms(150);
        pwm_set_gpio_level(buzzer1, 0);   // silencia
        pwm_set_gpio_level(buzzer2, 0);   // silencia
        sleep_ms(150);
        pwm_set_gpio_level(buzzer1, 250); // 50%
        pwm_set_gpio_level(buzzer2, 250);
        sleep_ms(150);
        pwm_set_gpio_level(buzzer1, 0);   // silencia
        pwm_set_gpio_level(buzzer2, 0);   // silencia
        sleep_ms(150);
        pwm_set_gpio_level(buzzer1, 250); // 50%
        pwm_set_gpio_level(buzzer2, 250);
        sleep_ms(150);
        pwm_set_gpio_level(buzzer1, 0);   // silencia
        pwm_set_gpio_level(buzzer2, 0);   // silencia
        sleep_ms(150);
        pwm_set_gpio_level(buzzer1, 250); // 50%
        pwm_set_gpio_level(buzzer2, 250);
        sleep_ms(150);
        pwm_set_gpio_level(buzzer1, 0);   // silencia
        pwm_set_gpio_level(buzzer2, 0);   // silencia
        sleep_ms(150);

    }
    else{
        num0(0,0,0);
        num1(0,0,0);
        num2(0,0,0);
        num3(0,0,0);
    }
}   
void gpio_callback(uint gpio, uint32_t events) {
    absolute_time_t now = get_absolute_time();
    if (absolute_time_diff_us(last_interrupt_time, now) < 250000) return;
    last_interrupt_time = now;

    if(gpio == BOTAO_A){
        gpio_put(AZUL, true);
    }
}

void inicia_pinos(){
    //Inicializa todos os tipos de bibliotecas stdio padrão presentes que estão ligados ao binário.
    stdio_init_all();
    sleep_ms(50);
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
  
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ENDERECO_I2C, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
  
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
    adc_init();
    adc_gpio_init(VERTICALY);
    adc_gpio_init(HORIZONTALX);
    pwm_init_gpio(buzzer1, PWM_WRAP, PWM_CLK_DIV);
    pwm_set_gpio_level(buzzer1, 0);  // Inicialmente desligado
  
    pwm_init_gpio(buzzer2, PWM_WRAP, PWM_CLK_DIV);
    pwm_set_gpio_level(buzzer2, 0);  // Inicialmente desligado  
    gpio_set_dir(VERDE, GPIO_OUT);
    gpio_init(AZUL);
    gpio_set_dir(AZUL, GPIO_OUT);
    gpio_init(VERMELHO);
    gpio_set_dir(VERMELHO, GPIO_OUT);
  
    gpio_init(SELEC);
    gpio_set_dir(SELEC, GPIO_IN);
    gpio_pull_up(SELEC);
  
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);
}
// Função principal
int main()
{
    inicia_pinos();
    //Inicializa a arquitetura do cyw43
    while (cyw43_arch_init())
    {
        printf("Falha ao inicializar Wi-Fi\n");
        sleep_ms(100);
        return -1;
    }

    // GPIO do CI CYW43 em nível baixo
    cyw43_arch_gpio_put(LED_PIN, 0);

    // Ativa o Wi-Fi no modo Station, de modo a que possam ser feitas ligações a outros pontos de acesso Wi-Fi.
    cyw43_arch_enable_sta_mode();

    // Conectar à rede WiFI - fazer um loop até que esteja conectado
    printf("Conectando ao Wi-Fi...\n");
    while (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 20000))
    {
        printf("Falha ao conectar ao Wi-Fi\n");
        sleep_ms(100);
        return -1;
    }
    printf("Conectado ao Wi-Fi\n");

    // Caso seja a interface de rede padrão - imprimir o IP do dispositivo.
    if (netif_default)
    {
        printf("IP do dispositivo: %s\n", ipaddr_ntoa(&netif_default->ip_addr));
    }

    // Configura o servidor TCP - cria novos PCBs TCP. É o primeiro passo para estabelecer uma conexão TCP.
    struct tcp_pcb *server = tcp_new();
    if (!server)
    {
        printf("Falha ao criar servidor TCP\n");
        return -1;
    }

    //vincula um PCB (Protocol Control Block) TCP a um endereço IP e porta específicos.
    if (tcp_bind(server, IP_ADDR_ANY, 80) != ERR_OK)
    {
        printf("Falha ao associar servidor TCP à porta 80\n");
        return -1;
    }

    // Coloca um PCB (Protocol Control Block) TCP em modo de escuta, permitindo que ele aceite conexões de entrada.
    server = tcp_listen(server);

    // Define uma função de callback para aceitar conexões TCP de entrada. É um passo importante na configuração de servidores TCP.
    tcp_accept(server, tcp_server_accept);
    printf("Servidor ouvindo na porta 80\n");
    bool cor = true;
    char str_T[5]; 
    char str_U[5];
    while (true)
    {
        /* 
        * Efetuar o processamento exigido pelo cyw43_driver ou pela stack TCP/IP.
        * Este método deve ser chamado periodicamente a partir do ciclo principal 
        * quando se utiliza um estilo de sondagem pico_cyw43_arch 
        */
        adc_incremental(&temperatura, &umidade);
        analise();
        cyw43_arch_poll(); // Necessário para manter o Wi-Fi ativo
        sleep_ms(100);      // Reduz o uso da CPU
        printf("Temperatura: %.2f C | Umidade: %.2f %%\n", temperatura, umidade);
        ssd1306_fill(&ssd, !cor);                          // Limpa o display
        ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor);      // Desenha um retângulo

        ssd1306_line(&ssd, 3, 25, 122, 25, cor); // (y = 31 aproximadamente no meio de 3 a 60)
        ssd1306_line(&ssd, 63, 3, 63, 60, cor); // (x = 63 aproximadamente no meio de 3 a 122)
        ssd1306_draw_string(&ssd, "Temp.", 10, 7);  // Desenha uma string 
        ssd1306_draw_string(&ssd, "Umidade", 67, 7);  // Desenha uma string 
        sprintf(str_T, "%1.0f", temperatura);
        sprintf(str_U, "%1.0f", umidade);
        ssd1306_draw_string(&ssd, str_T, 10, 31);  // Desenha uma string 
        ssd1306_draw_string(&ssd, str_U, 67, 31);  // Desenha uma string 
        ssd1306_send_data(&ssd);
    }

    //Desligar a arquitetura CYW43.
    cyw43_arch_deinit();
    return 0;
}

// -------------------------------------- Funções ---------------------------------



// Função de callback ao aceitar conexões TCP
static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
    tcp_recv(newpcb, tcp_server_recv);
    return ERR_OK;
}

// Tratamento do request do usuário - digite aqui
void user_request(char **request){

    if (strstr(*request, "GET /blue_on") != NULL)
    {
        //gpio_put(LED_BLUE_PIN, 1);
    }
    else if (strstr(*request, "GET /blue_off") != NULL)
    {
        //gpio_put(LED_BLUE_PIN, 0);
    }
    else if (strstr(*request, "GET /green_on") != NULL)
    {
        //gpio_put(LED_GREEN_PIN, 1);
    }
    else if (strstr(*request, "GET /green_off") != NULL)
    {
        //gpio_put(LED_GREEN_PIN, 0);
    }
    else if (strstr(*request, "GET /red_on") != NULL)
    {
        //gpio_put(LED_RED_PIN, 1);
    }
    else if (strstr(*request, "GET /red_off") != NULL)
    {
        //gpio_put(LED_RED_PIN, 0);
    }
    else if (strstr(*request, "GET /on") != NULL)
    {
        cyw43_arch_gpio_put(LED_PIN, 1);
    }
    else if (strstr(*request, "GET /off") != NULL)
    {
        cyw43_arch_gpio_put(LED_PIN, 0);
    }
};


// Função de callback para processar requisições HTTP
static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    if (!p)
    {
        tcp_close(tpcb);
        tcp_recv(tpcb, NULL);
        return ERR_OK;
    }

    // Alocação do request na memória dinámica
    char *request = (char *)malloc(p->len + 1);
    memcpy(request, p->payload, p->len);
    request[p->len] = '\0';

    printf("Request: %s\n", request);

    // Tratamento de request - Controle dos LEDs
    user_request(&request);
    


    // Cria a resposta HTML
    char html[2048];

    // Instruções html do webserver
    snprintf(html, sizeof(html),
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "\r\n"
    "<!DOCTYPE html>\n"
    "<html>\n"
    "<head>\n"
    "  <title>Embarcatech</title>\n"
    "  <style>\n"
    "    body { font-family: Arial, sans-serif; background-color: #f4f4f4; color: #333; }\n"
    "    h1 { text-align: center; }\n"
    "    .container { width: 80%%; margin: auto; overflow: hidden; }\n"
    "    .button { padding: 10px 20px; margin: 10px; border: none; border-radius: 5px; cursor: pointer; }\n"
    "    .blue { background-color: #007BFF; color: white; }\n"
    "    .green { background-color: #28A745; color: white; }\n"
    "    .red { background-color: #DC3545; color: white; }\n"
    "    .orange { background-color: #FFC107; color: white; }\n"
    "    .status { text-align: center; font-size: 1.5em; margin: 20px 0; }\n"
    "  </style>\n"
    "</head>\n"
    "<body>\n"
    "  <div class='container'>\n"
    "    <h1>Monitoramento Ambiental</h1>\n"
    "    <div class='status'>\n"
    "      <p>Temperatura: %.1f &deg;C</p>\n"
    "      <p>Umidade: %.1f &#37;</p>\n"
    "    </div>\n"
    "    <div style='text-align: center;'>\n"
    "      <button class='button blue' onclick='location.href=\"/ventilador_on\"'>Ligar/Desligar Ventilador</button>\n"
    "      <button class='button blue' onclick='location.href=\"/ventilador_off\"'>Ligar/Desligar Aquecedor</button>\n"
    "      <button class='button green' onclick='location.href=\"/umidificador_on\"'>Ligar/Desligar Umidificador</button>\n"
    "      <button class='button green' onclick='location.href=\"/umidificador_off\"'>Ligar/Desligar Desumidificador</button>\n"
    "    </div>\n"
    "  </div>\n"
    "</body>\n"
    "</html>\n",
    temperatura, umidade
  );  

    // Escreve dados para envio (mas não os envia imediatamente).
    tcp_write(tpcb, html, strlen(html), TCP_WRITE_FLAG_COPY);

    // Envia a mensagem
    tcp_output(tpcb);

    //libera memória alocada dinamicamente
    free(request);
    
    //libera um buffer de pacote (pbuf) que foi alocado anteriormente
    pbuf_free(p);

    return ERR_OK;
}