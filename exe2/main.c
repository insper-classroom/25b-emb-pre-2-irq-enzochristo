#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>


const int LED_PIN = 4;
const int BTN_PIN = 28;

int volatile cnt = 0 ;

void callback(uint gpio, uint32_t events){

  // so precisa de um evento, pq eh o caso de identificar apenas quando le
  if(events == 0x04){
    cnt = ! cnt ;
  }

}

int main() {
  stdio_init_all();

  gpio_init(LED_PIN);
  gpio_init(BTN_PIN);

  gpio_set_dir(LED_PIN, GPIO_OUT);
  gpio_set_dir(BTN_PIN, GPIO_IN);

  gpio_pull_up(BTN_PIN);


  // definindo a parte do codigo de identificacao de mudanca de estado IRQ

  gpio_set_irq_enabled_with_callback(BTN_PIN, GPIO_IRQ_EDGE_FALL , true, &callback);


  while (true) {
    // como eu faco a alteracao de estado no callback, aqui, eu apenas monitoro o fato de ligar ou desligar efetivamente o led.
    gpio_put(LED_PIN, (cnt));


    // isso nao eh mais necessario, uma vez que sempre que clicar, o valor vai se alterar e o checamento eh feito automaticamente
    // pelo sistema, pois quando clica vai do 0 para 1 e de 1 para zero.

    // por que esse codigo nao funciona perfeitamente ? 

    // if (cnt == 1)
    // {
    //   // ligado
    //   gpio_put(LED_PIN, (cnt == 1));
    //   // printf("%s", "ligou");
    // }else
    // {
    //    gpio_put(LED_PIN, (cnt == 2));
    //   // printf("%s", "desligou");
    //    cnt = 0;
    // }



  }
}


