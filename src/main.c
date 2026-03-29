/**
  ******************************************************************************
  * @file    main.c
  * @author  Marcos Zuccolotto
  * @version V1.0
  * @date   06/08/16
  * @brief   Exemplo de uso de timers com interrupcao
  ******************************************************************************
*/

#include "stm32f4xx.h"

#define PSC_1us (16000-1)
#define ARR_10ms (10-1)
#define ARR_250ms (250-1)
#define ARR_1s 999

#define LedPin GPIO_ODR_ODR_5
#define Bt1Pin GPIO_IDR_IDR_13

//! Manipulador interupcao TIM10
void TIM1_UP_TIM10_IRQHandler (void)
	{
	// Limpa o flag de interrupcao
	TIM10->SR &= ~TIM_SR_UIF;
	GPIOA->ODR^=LedPin;  // pisca o led
	}

int main(void)
{
	/*
	 * Variaveis do escopo do main
	 */

  /* Inicializa os perifericos */
  // Portas de entrada e saida
	// Habilitando clock GPIO
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN|RCC_AHB1ENR_GPIOBEN|RCC_AHB1ENR_GPIOCEN|RCC_AHB1ENR_GPIODEN|RCC_AHB1ENR_GPIOHEN;

	// Configura GPIOA como saida
	GPIOA->MODER&=~(GPIO_MODER_MODER5);
	GPIOA->MODER|=(GPIO_MODER_MODER5_0);

	// Configura GPIOB como entrada
	GPIOA->MODER&=~(GPIO_MODER_MODER5);
	GPIOA->MODER|=(GPIO_MODER_MODER5_0);

	/**
     * Configurando Timer 10
     */
	RCC->APB2ENR|=RCC_APB2ENR_TIM10EN;  // Habilita clocl para timer 10
	TIM10->PSC=PSC_1us;				// Configura a temporização
	TIM10->ARR=ARR_250ms;
	TIM10->CR1=TIM_CR1_ARPE|TIM_CR1_CEN; // Habilita o timer e o modo de autoreload
	TIM10->DIER|=TIM_DIER_UIE;             // habilita interrupcao por update

	// Habilita interrupção no controlador
	NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);//
	NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 3);//Priority 3


  while (1) // Laco principal
  {

	  if (GPIOC->IDR&Bt1Pin)
		  TIM10->ARR=ARR_10ms;
	  else
		  TIM10->ARR=ARR_250ms;
  }//

}// fim main
