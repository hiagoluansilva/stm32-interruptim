# STM32 InterrupTIM — Interrupção de Timer com ARR Dinâmico

🇧🇷 **Português** | 🇺🇸 [English](#english)

---

## Português

Demonstração de interrupção de timer no STM32F4xx com TIM10, mostrando como alterar o ARR em tempo de execução para mudar a frequência.

### O que faz
- **TIM10** gera interrupções periódicas com **PSC = 15999**
- Botão alterna entre dois períodos: **ARR = 9** (10 ms) e **ARR = 249** (250 ms)
- Prioridade NVIC configurada em **nível 3**
- Demonstra reconfiguração dinâmica de timer sem reiniciar o periférico

### Configuração do Timer
```
PSC = 15999 → clock do timer = 84 MHz / 16000 = 5250 Hz

ARR = 9:   período = 10/5250 ≈ 10 ms  → LED pisca a 100 Hz
ARR = 249: período = 250/5250 ≈ 250 ms → LED pisca a 4 Hz
```

### NVIC
- Prioridade TIM10: **3**
- Subprioridade: 0

### Microcontrolador
STM32F4xx — Atollic TrueSTUDIO

---

## English

Timer interrupt demonstration on STM32F4xx with TIM10, showing how to change ARR at runtime to alter frequency.

### What it does
- **TIM10** generates periodic interrupts with **PSC = 15999**
- Button toggles between two periods: **ARR = 9** (10 ms) and **ARR = 249** (250 ms)
- NVIC priority configured at **level 3**
- Demonstrates dynamic timer reconfiguration without restarting the peripheral

### Timer configuration
```
PSC = 15999 → timer clock = 84 MHz / 16000 = 5250 Hz

ARR = 9:   period = 10/5250 ≈ 10 ms  → LED blinks at 100 Hz
ARR = 249: period = 250/5250 ≈ 250 ms → LED blinks at 4 Hz
```

### NVIC
- TIM10 priority: **3**
- Subpriority: 0

### MCU
STM32F4xx — Atollic TrueSTUDIO
