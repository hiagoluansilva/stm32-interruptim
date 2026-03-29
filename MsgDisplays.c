/*
 * @file main.c
 * @author Marcos Zuccolotto
 * @date Jul2018
 * @version 1.0
 * @brief Acionando display 7 segmentos
 * */

/* Includes */
#include "stm32f4xx.h"
#define DISPLAY_NIBBLE // Decodificacao do display separado em 2 nibbles
#include "dec7seg.h"

// Tempos
// fosc = 16MHz
#define PSC_T10 159  //! Saida do prescaller em 100kHz - 10us
#define ARR_T10 99	 //! Estouro do TIM10 em 1ms - 10 000us
#define TLED	100	 //! Tempo de mudanca estado led
#define TDSPL	300

// Definicões de pinos
#define LedPin      GPIO_ODR_ODR_5  //! Pino do Led
#define LedPort		GPIOA			//! Porta do Led
#define ButtonPin	GPIO_IDR_IDR_13 //! Pino do botao
#define ButtonPort	GPIOC			//! Porta do botao
#define BUTTONPRESSED 0x00			//! Valor do botao pressionado - pino ativo em LOW

// Definicoes para uso display
// Pinos utilizados pelo display : PB 7,8,9,10 - 12,13,14,15
#define DsplDataPort 	GPIOB

#define DsplDataMaskHigh	0x0fff
#define DsplDataShiftHigh	0x08
#define DsplDataMaskLow	 	0xf87f
#define DsplDataShiftLow	0x07

#define DsplCtrlPort		GPIOC
#define DsplCtrlMask		0xfff3
#define DsplCtrlShift		0x02

#define NUMDISP 			4


char msg[]={"Este eh um dos trabalhos do 2 trimestre        "};


//! proximo caractere a ser exibido
    uint8_t iStr;
/* Private function prototypes -----------------------------------------------*/

uint16_t charTo7seg(unsigned char ch);



int main(void)
{
	uint8_t i;
	uint16_t TIMLed=0; //! Temporizador para o led
	uint8_t  TLedF=0;  //! Flag do temporizador do led

	uint8_t	statBt;		//! status do botão

	uint16_t TIMDisp=0;

	uint8_t segundos=0,
			segmentos;


	uint8_t msglen;

	uint8_t disp[NUMDISP]={0,0,0,0};
	uint8_t dispctrl;	//! display a ser acionado
	// Configurando GPIOs

	// Habilitando clock GPIO
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN|RCC_AHB1ENR_GPIOBEN|RCC_AHB1ENR_GPIOCEN|RCC_AHB1ENR_GPIODEN|RCC_AHB1ENR_GPIOHEN;

	// Configura GPIOA como saida
	GPIOA->MODER&=~(GPIO_MODER_MODER5);
	GPIOA->MODER|=(GPIO_MODER_MODER5_0);
	// Configura GPIOC como entrada
	GPIOC->MODER&=~(GPIO_MODER_MODER13);

	// Pinos utilizados pelo display como dados : PB 7,8,9,10 - 12,13,14,15
	GPIOB->MODER&=~(GPIO_MODER_MODER7|GPIO_MODER_MODER8|GPIO_MODER_MODER9|GPIO_MODER_MODER10);
	GPIOB->MODER&=~(GPIO_MODER_MODER12|GPIO_MODER_MODER13|GPIO_MODER_MODER14|GPIO_MODER_MODER15);
	GPIOB->MODER|=(GPIO_MODER_MODER7_0|GPIO_MODER_MODER8_0|GPIO_MODER_MODER9_0|GPIO_MODER_MODER10_0);
	GPIOB->MODER|=(GPIO_MODER_MODER12_0|GPIO_MODER_MODER13_0|GPIO_MODER_MODER14_0|GPIO_MODER_MODER15_0);

	// Pinos utilizados pelo display como controle : PC 2,3
	GPIOC->MODER&=~(GPIO_MODER_MODER2|GPIO_MODER_MODER3);
	GPIOC->MODER|=(GPIO_MODER_MODER2_0|GPIO_MODER_MODER3_0);


// Configurar o timer 10
	// Habilitando clock TIM10
	RCC->APB2ENR|=RCC_APB2ENR_TIM10EN;

	// Configurando TIM10
	TIM10->PSC=PSC_T10;
	TIM10->ARR=ARR_T10;
	TIM10->CR1=TIM_CR1_CEN; // Vai Timer 10 !

	DsplCtrlPort->ODR&=DsplCtrlMask; // Ativa o display 0

	dispctrl = -1;

    msglen=strlen(msg);

   iStr=0;


    for (i=0;i<NUMDISP;i++)
		disp[3-i]= charTo7seg(msg[iStr++]);




  /* Infinite loop */
  while (1)
  {

	 if (TIM10->SR&TIM_SR_UIF)// pooling TIM10
	 	 {
		 TIM10->SR&=~TIM_SR_UIF;  // Desligo o UIF para ver ele ligar novamente

		 //Soft timer do led
		 TIMLed++;
		 if (TIMLed==TLED) // Deu o tempo ?
		 	 {
			 TLedF=1; // Marca que o tempo passou !
			 TIMLed=0;
		 	 }


		 // apaga os displays
					 DsplDataPort->ODR&=DsplDataMaskHigh;
					 DsplDataPort->ODR&=DsplDataMaskLow;
					 dispctrl++;  // atualiza ponteiro para digito a ser exibido
					 if (dispctrl>=NUMDISP)
					 	 dispctrl=0;

					 DsplCtrlPort->ODR&=DsplCtrlMask;
					 DsplCtrlPort->ODR|=dispctrl<<DsplCtrlShift;

					 segmentos=dec7seg[1][disp[dispctrl]];
					 DsplDataPort->ODR|=segmentos<<DsplDataShiftHigh;
					 segmentos=dec7seg[0][disp[dispctrl]];
					 DsplDataPort->ODR|=segmentos<<DsplDataShiftLow;




		 //Soft timer do display
		 TIMDisp++;
		 if (TIMDisp==TDSPL) // Deu o tempo ?
		 	 {
			 TIMDisp=0; // Marca que o tempo passou !

			 segundos++;
			 if (segundos>1250)
				 segundos=0;

			 for (i=(NUMDISP-1);i>0;i--)
				 disp[i]= disp[i-1];

			 	 disp[0]= charTo7seg(msg[iStr++]);

	            if (msg[iStr]=='\0')
	            	iStr=0;

		 	 }


		 // Rotina de debouncing
		 if ((ButtonPort->IDR & ButtonPin)==BUTTONPRESSED)
			 statBt=1;
		 else
			 statBt=0;


	 	 }// fim pool TIM10


// Pisca o led
	 if (TLedF)
	 	 {
		 GPIOA->ODR^=LedPin;	 // Toggle do pino do led
		 TLedF=0;
	 	 }// Fim pisca led
  }// fim laco
}// fim main




uint16_t charTo7seg(unsigned char ch)
{

	uint8_t code;

	code = CODE_BLANK;

	if (ch>='0' && ch<='9')
		code=ch-'0';

	if (ch>='A' && ch<='Z')
			code=ch-'A'+DISPLAY_A;

	if (ch>='a' && ch<='z')
				code=ch-'a'+DISPLAY_A;

	return (code);

}
