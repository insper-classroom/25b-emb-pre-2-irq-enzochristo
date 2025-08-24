#include <stdio.h>
#include <time.h>
#include "hardware/gpio.h"
#include "pico/stdlib.h"


const int BTN_PIN_R = 28;

int volatile flag = 0;



void btn_callback(uint gpio, uint32_t events){
    if (events == 0x04)
    {
       

        // flag = start - 800 > 0.001;
        // flag =1;
    }
}

int main() {
    stdio_init_all();

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true, &btn_callback); // Sempre na main!

    while (true) {

        printf("%d", flag);
        // if (flag)
        // {
        //     printf("Aperto curto!\n");
        //     flag = 0;

        // }
      
        
    }
}
