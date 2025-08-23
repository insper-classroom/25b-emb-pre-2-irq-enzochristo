#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
int volatile flag = 2;

void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) { // fall edge
    flag = 0;
    
  } else if (events == 0x8) { // rise edge
    flag = 1;
  }
}

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_set_irq_enabled_with_callback(
      BTN_PIN_R, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);

    // se eu nao colocar o or, ele nao vai nos dois casos, 
    // ele so vai entrar no callback quando houver os dois casos de high e low no pino.

    // por que ele funciona no ex0, mas nao no ex1, somente com as duas condicoes.

  while (true) {

    if (flag == 1){
    printf("rise \n");  
      flag = 2;  
    }else if(flag == 0){
    printf("fall \n"); 
      flag = 2;   
    }else{

    }

  }
}
