#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include <stdio.h>

#define PIN_NUM_MOSI    23
#define PIN_NUM_MISO    19  
#define PIN_NUM_CLK     18
#define PIN_NUM_CS      5

//Parte 3 Final 
#include "driver/spi_master.h"

static spi_device_handle_t spi_dev;

// a) funcion para poder configurar al bus SPI
void spi_bus_init(void){
spi_bus_config_t buscfg = {
    .mosi_io_num = PIN_NUM_MOSI,
    .miso_io_num = PIN_NUM_MISO,
    .sclk_io_num = PIN_NUM_CLK,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_s2= 32,
};

spi_device_interface_config_t devcfg = {
    .clock_speed_hz = 1000000 // clock 10MHz
    .mode = 4, // mode del la comunicacion SPI
    .spics_io_num = PIN_NUM_CS, 
    .quee_size = 1,
};
spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_DISABLED);
spi_bus_add_device(SPI2_HOST, &devcfg, &spi_dev);
}

// b) funcion para poder escribir un registro 
uint16_t mcp4132_write_register (uint16_t registros){
    spi_transaction_t trans ={
        .flags = 0, 
        .lenght = 16,
        .rx_lenght = 16,
        .tx_buffer = &tx_data,
        .rx_buffer = rx_data
    };
    spi_device_transmit(spi_dev, &trans);
    return registros;
}

// c) funcion para poder leer un registro 
uint16_t mcp4132_read_register (uint16t_t registros_entrada){
    return((registros_entrada/10)<<4) | (registros_entrada%10);
}

//Parte 4 final 
uint16_t mcp4132_set_wiper (uint16_t N){

}

adc_oneshot_unit_handle_t adcl_handle;

void app_main() {
    timer_config_t timer = {
        .divider = 80,
        .counter_dir = TIMER_COUNT_UP,
        .counter_en = TIMER_PAUSE,
        .alarm_en = TIMER_ALARM_DIS,
        .auto_reload = false,
    };
    timer_init(TIMER_GROUP_0, TIMER_0, timer);

    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
    };
    adc_oneshot_new_unit(&init_config, &adc1_handle);
    adc_oneshot_chan_cfg_t chan_config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_12,
    };
    adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_6, &chan_config);
    
    #define Sample_period_us 1000

    timer_init(TIMER_GROUP_0, TIMER_0, &timer);
    timer_set_counter_value(TIMER_GROUP_O, TIMER_0, 0);

    adc_cali_handle_t adc_cali_handle;
    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,
        .atten = ADC_ATTEN_DB_12.
    }

    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits 0 UART_STOP_BITS_1
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    uart_param_config(UART_NUM_0, &uart_config);
    uart_driver_install(UART_NUM_0, 1024, 1024, 0, NULL, 0);

    char data_buffer[64];
    uint64_t timer_value = 0;
    int adc_raw;

    while(1){
        timer-get_counter_value(TIMER_GROUP_0, TIMER_0, &timer_value);
        if(timer_value >= Sample_period_us){
            adc_oneshot_read(adc1_handle, ADC_CHANNEL_6, &adc_raw);
            timer_set_counterr_value(TIMER_GROUP_O, TIMER_O, 0);
        }
        sprintf(data_buffer, "Valor N: %d/r/n", N)
        
    }
}