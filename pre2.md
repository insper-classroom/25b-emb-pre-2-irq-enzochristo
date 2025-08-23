## Pre Lab - 2 - Embarcados


O uso de eventos em programacao de sistemas eh algo bastante comum. Assim, define-se `funcoes` especificas para determinados eventos, que sao executadas quando esses eventos sao detectados. As funcoes/eventos podem ter fifetentes niveis de prioridade, sendo possivel controlar a ordem de execucao de cada uma delas. 

Dois exemplos de funcoes de eventos: 

- Uma funcao que eh acionada sempre que um dado chega pela porta ethernet 

- Pressionamento de um botao

- Eventos periodicos, como a execucao de uma funcao a cada x segundos, como por exemplo, para piscar um led. 


##### No contexto de embarcados ... 

Em sistemas embarcados, nao temos um OS tao desenvolvido ou ate mesmo, nem possui um. Assim, existem interrupcoes de hardware, que sao as proprias funcoes de eventos que vao ser executadas pelo microcontrolador em resposta a eventos detectados pelos perifericos. 

Um exemplo? 

-  Quando o botao for pressionado, alterando o estado de HIGH para LOW, uma funcao especifica, conhecida como `handler` sera executada. 

Isso por si so eh muito bom, pois nao temos mais a necessidade de ficar ultilizando o esquema de mudanca de estado por `pooling`, que eh o caso em que usamos o `while(1)`. 

`Pooling :`

```c

while(1){
  if(gpio_get(BTN))
    gpio_set(LED, 1);
  else
    gpio_set(LED, 0);
};

```

    Nesse exemplo de código, fica-se constantemente checando por alterações no registrador do GPIO responsável pelo botão, a fim de decidirmos se o LED ficará aceso ou apagado. O CORE está constantemente trabalhando para executar essas operações



#### IRQ (Interruption Request)
--- 
Eh o nome que se da quando uma interrupcao eh requisitada pelo hardware. Isso eh muito importante, pois permite que o `hardware notifique o software sobre um evento que necessita de antencao imediata. `Quando um dispositivo periferico precisa que o processador execute uma tarefa, ele envia um sinal de IRQ para o processador. 

O que acontece na pratica quando uma IRQ, ou seja uma requisicao para que o programa seja interrompido, acontece uma chamada especial de dois tipos especiais de funcoes `handler` ou `callback`.

Apos a interrupcao, o programa volta para seu "modo operandi" normal, que eh sem interrupcoes, voltando para o estado anteriormente salvo e retome o a execucao do ponto de onde foi interrompido. 


###### Tipo Volatile:

    Todas as variaveis que sao de interrupcao TEM, necessariamente, que ser do tipo volatile, caso contrario, vai gerar um erro no programa.

Sempre que uma interrupcao alterar uma variavel global, ela deve possuir o modificador 
[`volatile`](https://barrgroup.com/blog/how-use-cs-volatile-keyword).

Esse pragma serve para informar ao compliador que essa variavel sera modificada sem que o compilador saiba, evitando assim que a variavel nao seja otimizada incorretamente. 


    Somente ultilizar volativle quando necessario uma IRQ alterar o valor de uma variavel. Ultilizamos a variavel volatile dentro de algo que exige uma condicao, como um if. 


Um exemplo basico do seu uso : 



```c

/* flag */
volatile char g_but_flag; // (1)

/* funcao de callback/ Handler */
void btn_callback(uint gpio, uint32_t events) {
  g_but_flag = 1;
}

void main(void){
  /* Inicializacao */
  // ...
  // ...

  while(1){
  
   if (g_but_flag) {  // (2)
       pisca_led(5, 200);    
       g_but_flag = 0; // (3)
   }
  }
}

```


Um exemplo de codigo mais elaborado que liga e desliga a luz de um led eh: 

```c

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define LED_PIN 5 
#define BTN_PIN 26 

volatile int g_status = 0;

void gpio_callback(uint gpio, uint32_t events) {
    if (events == 0x4) {         // fall edge
        g_status = 1;
    } else if (events == 0x8) {  // rise edge
        
    }
}

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN);
  gpio_set_dir(BTN_PIN, GPIO_IN);
  gpio_pull_up(BTN_PIN);
  gpio_set_irq_enabled_with_callback(BTN_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  int led_status = 0;
  while (true) {
    if (g_status){
      g_status = 0; // clean IRS flag
      led_status = !led_status;
      gpio_put(LED_PIN, led_status);
    }
  }
}


```

Observe como a condicao se altera ao entrar no if e ao iniciar o programa.

##### Bordas:
---

As bordas se referem a mudancas no estado de um sinal eletrico, que acontece justamente por conta dessas IRQs, que estao gerenciando o processador para verificar se esta, ou nao, mudando de estado. 


- Tipos de bordas: 

    - `GPIO_IRQ_EDGE_FALL`: Descida (Botão apertado) - `Fall edge` 
    event: 0x04 - > 0x04, esta em hexadec. em binario: 0000 0010, indica para o processador que o botao desceu

    - `GPIO_IRQ_EDGE_RISE`: Subida (Botão solto) - `Rise edge`
    event: 0x08 -> 0000 0100, indica para o processador que o botao subiu

    - `GPIO_IRQ_EDGE_FALL` | GPIO_IRQ_EDGE_RISE: Subida e descida
    event: 0x04 + 0x08 -> Indica os dois casos, entao por isso a soma e o `ou`.



 - `gpio_set_irq_enabled_with_callback` : (o pino que o GPIO sera configurado, mascara de evento que define o tipo de interrupcao, true(habilita a interrupcao), ponteiro da funcao de estado)


O motivo de ser o `ponteiro` da funcao e nao o nome dela explicitamente : 

    Em C e C++, o nome de uma função pode, em alguns contextos, ser tratado como um ponteiro para a própria função. No entanto, o uso explícito do operador de endereço (&) é uma boa prática para deixar claro que você está passando o endereço de memória da função e não a executando. Isso é essencial, porque o objetivo não é chamar a função naquele momento, mas sim registrar o local dela para que seja executada futuramente, quando a interrupção ocorrer.



#### Multiplos Callbacks: 
---

So podemos fazer um unico callback para os GPIOS da pico, devemos usar a informacao do argumento `gpio` para sabermos qual pino estamos lidando dentro do callback

Para o caso em que estivermos trabalhando com multiplos callbacks, mas em gpios diferentes, isto eh, em pinos diferentes, mas funcoes iguais, temos que ultilizar duas funcoes do gpio: 

- `gpio_set_irq_enabled_with_callback` - > define tudo que foi apontado anteriormente

- `gpio_set_irq_enabled` - > Apenas muda o pino que esta sendo referenciado para caso aconteca algo chame a funcao por alteracao nele, mas vai ultilizar a mesma funcao para a interrupcao que foi setada na linha de cima. 

O codigo abaixo ilustra bem: 


```c

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

const int BTN_PIN_R = 28; 
const int BTN_PIN_G = 26; 
const int LED_PIN_R = 4; 
const int LED_PIN_G = 6; 

volatile int g_flag_r = 0;
volatile int g_flag_g = 0;

void pin_init(void);

void btn_callback(uint gpio, uint32_t events) {
    if (gpio == BTN_PIN_R) {     
        g_flag_r = 1;
    } else if (gpio == BTN_PIN_G) {
        g_flag_g = 1;
    }
}

int main() {
  stdio_init_all();
  pin_init();

  gpio_set_irq_enabled_with_callback(BTN_PIN_R,
                                     GPIO_IRQ_EDGE_FALL,
                                     true,
                                     &btn_callback);
  // Segunda IRQ usa callback já configurado.
  gpio_set_irq_enabled(BTN_PIN_G,
                        GPIO_IRQ_EDGE_FALL,
                        true);

  while (true) {
    if(g_flag_r || g_flag_g) {
      printf("IRQ 0: %d | IRQ 1: %d\n", g_flag_r, g_flag_g);
      
      // clear flags
      g_flag_r = 0;
      g_flag_g = 0;
    }
  }
}

void pin_init(void){
  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_init(BTN_PIN_G);
  gpio_set_dir(BTN_PIN_G, GPIO_IN);
  gpio_pull_up(BTN_PIN_G);
}

```