#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
// volatile int capture_flag = 0;
// int btn_flag;

void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) { // fall edge

    // printf("btn pressed \n");
    
    while (!gpio_get(BTN_PIN_R)) {
      // sleep_ms(1);
    }
    

    
    // printf("btn released \n");
    // sleep_ms(1);


    // capture_flag = 1;
    // btn_flag = 1;
  }
}



int main() {
  stdio_init_all();
  // pin_init();

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true,
                                     &btn_callback);

  // se alterar o estado, ao clicar no botao, mesmo que esteja no loop ele vai ser direcionado
  // para a funcao callback. Dessa forma, vai alterar o estado da flag
  // por isso, nao precisamos chamar ela quando alteramos o estado da flag. 

  // O que faz chamar a funcao callback eh a alteracao na tensao do pino definido como gpio. 

  // Usamos flags quando estamos fazendo alguma alteracao de estado. 

  // Nesse caso, o estado vai e volta automaticamente, entao nao precisamos fazer
  // nada no formato true/false. 

  // Tampouco precisamos de while, pois como o caso eh automatico, o pull up acontece normalmente
  // Apenas a variacao do sinal de entrada ja vai fazer com que o callback entre em acao e faca o print. 

  // int a;
  // while (1) {
  //   // if (capture_flag) {
  //   //   capture_flag = 0;
  //   //   // btn_flag = 0;
  //   // }


  // }
}