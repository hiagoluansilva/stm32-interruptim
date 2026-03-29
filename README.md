# stm32-interruptim

Exemplo de timer com interrupção no STM32F4xx — pisca LED e ajuste de velocidade por botão.

## Descrição

Projeto didático que demonstra o uso do TIM10 com interrupção (IRQ) para piscar um LED. A frequência de pisca é alterada dinamicamente via botão: ao pressionar PC13, o LED passa de 250 ms para 10 ms (pisca mais rápido).

## Hardware

- Microcontrolador: STM32F4xx
- LED: PA5
- Botão: PC13

## Configuração do Timer

```c
TIM10->PSC = PSC_1us;   // 16000-1 → resolução de 1 µs
TIM10->ARR = ARR_250ms; // 250 contagens → 250 ms (normal)
//           ARR_10ms   // 10 contagens → 10 ms (botão pressionado)
```

## ISR do Timer

```c
void TIM1_UP_TIM10_IRQHandler(void) {
    TIM10->SR &= ~TIM_SR_UIF;  // Limpa flag
    GPIOA->ODR ^= LedPin;      // Toggle LED
}
```

## Conceitos demonstrados

- Configuração de interrupção de timer (TIM10 → `TIM1_UP_TIM10_IRQn`)
- NVIC: `NVIC_EnableIRQ` e `NVIC_SetPriority`
- Alteração do `ARR` em tempo de execução

## IDE

Atollic TrueSTUDIO 9.3

## Autor

Prof. Marcos Zuccolotto — Centro Tecnológico Liberato
