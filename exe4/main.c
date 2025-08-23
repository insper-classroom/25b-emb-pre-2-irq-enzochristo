#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>


const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;

const int RED_PIN = 4;
const int GREEN_PIN = 6;


int volatile red_flag = 0;
int volatile green_flag = 0;


void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x04)
  {
    red_flag = ! red_flag;
  }
  else if (events == 0x08)
  {
    green_flag = ! green_flag;
  }
}

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_init(BTN_PIN_G);
  gpio_set_dir(BTN_PIN_G, GPIO_IN);
  gpio_pull_up(BTN_PIN_G);

  gpio_init(RED_PIN);
  gpio_set_dir(RED_PIN, GPIO_OUT);

  gpio_init(GREEN_PIN);
  gpio_set_dir(GREEN_PIN, GPIO_OUT);


  // red, somente quando desce
  gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true,&btn_callback);

  // green, somente quando sobe
  gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_RISE, true);



  while (true) {
    gpio_put(RED_PIN, red_flag);
    gpio_put(GREEN_PIN, green_flag);
  }
}
