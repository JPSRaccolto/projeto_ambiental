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
#include "ws2812.pio.h"          //Manipula a matriz de LED
#include "hardware/i2c.h"        //Necessário para a comunicação com ssd1306
#include "ssd1306.h"             //inicia o ssd1306
#include "font.h"                //Fonte de palavras a serem escritas no ssd1306

// Credenciais WIFI - Tome cuidado se publicar no github!
#define WIFI_SSID "Galaxy S20 FE 5G"
#define WIFI_PASSWORD "abcd9700"

// Definição dos pinos dos LEDs
#define LED_PIN CYW43_WL_GPIO_LED_PIN   // GPIO do CI CYW43
#define BOTAO_A 5
#define BOTAO_B 6
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
float temperatura = 25.0f;
float umidade = 50.0f;
bool temp1 = false;
bool temp2 = false;
bool temp3 = false;
bool temp4 = false;
bool global1 = false;
bool global2 = false;
bool global3 = false;
bool global4 = false;
bool ventilador_ligado = false;
bool umidificador_ligado = false;
bool desumidificador_ligado = false;
bool aquecedor_ligado = false;
absolute_time_t ultimo_bip;
bool estado_buzzer = false;
bool buzzer_ativo = false;
int ciclos_buzzer = 0; // quantos bip ON/OFF já ocorreram

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

void atualiza_buzzer() {
    if (!buzzer_ativo) return;

    if (absolute_time_diff_us(ultimo_bip, get_absolute_time()) >= 250000) { // 250ms
        ultimo_bip = get_absolute_time();
        estado_buzzer = !estado_buzzer;
        pwm_set_gpio_level(buzzer1, estado_buzzer ? 250 : 0);
        pwm_set_gpio_level(buzzer2, estado_buzzer ? 250 : 0);

        ciclos_buzzer++;
        if (ciclos_buzzer >= 8) {  // 4 ciclos ON/OFF = 1 segundo
            buzzer_ativo = false;
            pwm_set_gpio_level(buzzer1, 0);
            pwm_set_gpio_level(buzzer2, 0);
            ciclos_buzzer = 0;
        }
    }
}

void analise(){
    if(temperatura > 55.0f){
        global1 = !global1;
        if(temp1 == false ){
            if(global1 == true){
                printf("Temperatura elevada, por favor ligue o ventilador\n");    
        }
    }
        num1(255,0,0);
        if (!buzzer_ativo) {
            buzzer_ativo = true;
            ultimo_bip = get_absolute_time();
            ciclos_buzzer = 0;
        }
        //colocar o liga/desliga no webserver
    }
    else if(temperatura < 5.0f){
        temp4 = !temp4;
        printf("Sistema operando em temperaturas críticas, ligue o aquecedor\n");
        num0(0,0,255);
        if (!buzzer_ativo) {
            buzzer_ativo = true;
            ultimo_bip = get_absolute_time();
            ciclos_buzzer = 0;
        }

    }
    else if(umidade > 70.0f){
        temp3 = !temp3;
        printf("Umidade muitoo elevada, ligue o desumidificador\n");
        num3(0,255,255);
        if (!buzzer_ativo) {
            buzzer_ativo = true;
            ultimo_bip = get_absolute_time();
            ciclos_buzzer = 0;
        }

    }
    else if(umidade < 30.0f){
        global2 = !global2;
        if(temp2 == false){
            if(global2 == true){
                printf("Umidade em situação crítica, ligue o umidificador\n");
            }
        }
        num2(255,165,0);
        if (!buzzer_ativo) {
            buzzer_ativo = true;
            ultimo_bip = get_absolute_time();
            ciclos_buzzer = 0;
        }

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
        temp1 = !temp1;
        ventilador_ligado = !ventilador_ligado;
    }
    if(gpio == BOTAO_B){
        temp2 = !temp2;
        umidificador_ligado = !umidificador_ligado;
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
    gpio_init(VERDE); 
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
    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(BOTAO_B, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
}
void liga(){
    if (ventilador_ligado == true) {
        gpio_put(AZUL, true);
        if(temperatura >= 31.0f){
            printf("Ventilador ligado manualmente!\n");
            temperatura -= 0.5f;
        }
        else{
            ventilador_ligado = false;
            gpio_put(AZUL, false);
            temp1 = false;
        }
    }

    if(umidificador_ligado == true){
        gpio_put(VERDE, true);
        if(umidade <= 35.0f){
            umidade += 0.5f;
            printf("Umidificador ligado manualmente!\n");
        }
        else{
            umidificador_ligado = false;
            gpio_put(VERDE, false);
            temp2 = false;
        }
    }
    
    if(aquecedor_ligado == true){
        gpio_put(VERMELHO, true);
        if(temperatura <= 25.0f){
            temperatura += 0.5f;
            printf("Aquecedor ligado manualmente\n");
        }
        else{
            aquecedor_ligado = false;
            gpio_put(VERMELHO,false);
            temp4 = false;
        }
    }
    
    if(desumidificador_ligado == true){
        gpio_put(VERDE,true);
        gpio_put(VERMELHO, true);
        if(umidade >= 60.0f){
            umidade -= 0.5f;
            printf("Desumidificador ligado manualmente\n");
        }
        else{
            desumidificador_ligado = false;
            gpio_put(VERDE, false);
            gpio_put(VERMELHO, false);
            temp3 = false;
        }
    }
}
// Função principal
int main()
{
    inicia_pinos();
    atualiza_buzzer();

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
        atualiza_buzzer();
        adc_incremental(&temperatura, &umidade);
        liga();
        analise();
        cyw43_arch_poll(); // Necessário para manter o Wi-Fi ativo
        sleep_ms(200);      // Reduz o uso da CPU
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

const char *get_alert_messages() {
    static char alert[512];
    alert[0] = '\0';

    if (temp1 == true)
        strcat(alert, "<p style='color:red;'> Temperatura elevada - Ligue o ventilador!</p>");
    if (temp2 == true)
        strcat(alert, "<p style='color:orange;'> Umidade baixa - Ligue o umidificador!</p>");
    if (temp3 == true)
        strcat(alert, "<p style='color:blue;'> Umidade alta - Ligue o desumidificador!</p>");
    if (temp4 == true)
        strcat(alert, "<p style='color:purple;'> Temperatura muito baixa - Ligue o aquecedor!</p>");

    if (alert[0] == '\0')
        strcpy(alert, "<p style='color:green;'> Sistema operando normalmente.</p>");

    return alert;
}


// Tratamento do request do usuário - digite aqui
void user_request(char **request){
    if (strstr(*request, "GET /ventilador") != NULL) {
        ventilador_ligado = !ventilador_ligado;
        temp1 = !temp1;
    }
    else if (strstr(*request, "GET /aquecedor") != NULL) {
        aquecedor_ligado = !aquecedor_ligado;

    }
    else if (strstr(*request, "GET /umidificador") != NULL) {
        umidificador_ligado = !umidificador_ligado;
        temp2 = !temp2;
    }
    else if (strstr(*request, "GET /desumidificador") != NULL) {
        desumidificador_ligado = !desumidificador_ligado;

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
    

    adc_incremental(&temperatura, &umidade);
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
    "    .alert-box { border: 2px solid #444; background: #ffe; padding: 10px; margin: 10px; }\n"
    "  </style>\n"
    "</head>\n"
    "<body>\n"
    "  <div class='container'>\n"
    "    <h1>Monitoramento Ambiental</h1>\n"
    "    <div class='alert-box'>\n"
    "      %s\n"
    "    </div>\n"
    "    <div class='status'>\n"
    "      <p>Temperatura: %.1f &deg;C</p>\n"
    "      <p>Umidade: %.1f &#37;</p>\n"
    "    </div>\n"
    "    <div style='text-align: center;'>\n"
    "      <button class='button blue' onclick='location.href=\"/ventilador\"'> Ventilador</button>\n"
    "      <button class='button blue' onclick='location.href=\"/aquecedor\"'>Aquecedor</button>\n"
    "      <button class='button green' onclick='location.href=\"/umidificador\"'>Umidificador</button>\n"
    "      <button class='button green' onclick='location.href=\"/desumidificador\"'>Desumidificador</button>\n"
    "    </div>\n"
    "  </div>\n"
    "</body>\n"
    "</html>\n",
    get_alert_messages(), temperatura, umidade
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