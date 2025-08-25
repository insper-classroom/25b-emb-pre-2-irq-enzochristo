#include <stdio.h>
#include <time.h>
#include "hardware/gpio.h"
#include "pico/stdlib.h"


const int BTN_PIN_R = 28;
time_t sec;
time_t stop;
int volatile flag = 1;
int cnt = 8;



void btn_callback(uint gpio, uint32_t events){
    if (events == 0x04)
    {
        flag = 0;
    }

    else if (events == 0x08){
        flag = 1;
    }
}

int main() {
    stdio_init_all();

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL , true, &btn_callback); // Sempre na main!

    while (true) {
        
        if (!flag){
            for (int i=0; i <cnt; i++){
                sleep_ms(100);
                if(flag == 1){
                    break;
                }
            }
            if (!flag)
            {
                printf("Aperto longo!\n");
                flag = 2;
            }
            else if (flag ==1)
            {
                printf("Aperto curto!\n");
                flag = 2;
            }
            
        }

        // if(!flag){
        //     flag = 2;
        // }
        
      
        
    }
}
