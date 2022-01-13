#include "drivers.h"
/*	Basic initialisation	*/
void startup(){
	initClock();
	initTIM17();
	initUSART2();
	msDelay(500);
	printU2("Startup completed\n");
}

/* Maxing out the device clock */
void initClock(){
	// cpu 72MHz
	FLASH->ACR |= FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_1;
	// APB1 (low)		: 36MHz
	// APB2 (high)	: 72MHz
	RCC->CR |= (RCC_CR_HSEBYP | RCC_CR_HSEON);
	while(!(RCC->CR & RCC_CR_HSERDY)){}
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
	//HSE->PLL : 72 MHz
	RCC->CFGR |= (RCC_CFGR_PLLMUL12);	
	RCC->CFGR |= RCC_CFGR_PLLSRC_HSE_PREDIV;		
	RCC->CR |= RCC_CR_PLLON;		
	while(!(RCC->CR & RCC_CR_PLLRDY));
	RCC->CFGR |= RCC_CFGR_SW_PLL;		
}
/* Initialisation of USART2 for printing function debugging */
void initUSART2(){
	//GPIOs
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER2_1 | GPIO_MODER_MODER15_1;
	GPIOA->AFR[0] |= 7U<<GPIO_AFRL_AFRL2_Pos;
	GPIOA->AFR[1] |= 7U<<GPIO_AFRH_AFRH7_Pos;
	//Usart @ 115200
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	USART2->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
	USART2->BRR = 312;
}
/* Print UART to windows computer (or whatever computer) */
void printU2(char* data){
	int i;
	int size = (int) strlen(data);
	for(i=0;i<size;i++){
		USART2->TDR = data[i];
		while(!(USART2->ISR & USART_ISR_TXE)){}
	}
}
/* Initialisation of TIM17 as base for msDelay and usDelay */
void initTIM17(){
	//for msDelay and usDelay
	RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;
	TIM17->CR1 |= TIM_CR1_ARPE | TIM_CR1_OPM;
}

void msDelay(uint16_t delay){
	TIM17->CR1 &= ~TIM_CR1_CEN;
	TIM17->SR =0U;
	TIM17->CNT =0U;
	TIM17->PSC = 7199U;
	TIM17->ARR = delay*10U-1U;
	TIM17->CR1 |= TIM_CR1_CEN;
	while(!(TIM17->SR & TIM_SR_UIF)){}
}

void usDelay(uint16_t delay){
	TIM17->CR1 &= ~TIM_CR1_CEN;
	TIM17->SR =0U;
	TIM17->CNT =0U;
	TIM17->PSC = 71U;
	TIM17->ARR = delay-1U;
	TIM17->CR1 |= TIM_CR1_CEN;
	while(!(TIM17->SR & TIM_SR_UIF)){}
}

/* Basic timer drivers */
void initTIM(uint32_t tim, uint32_t psc, uint32_t arr, uint8_t irqUsed){ //for timers 2,3,15,16
	TIM_TypeDef* TIM;
	IRQn_Type TIM_IRQn;
	switch(tim){
		case 2U:	{	TIM=TIM2; 
							RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
							TIM_IRQn = TIM2_IRQn;
							break;}				
		case 3U:	{	TIM=TIM3; 
							RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
							TIM_IRQn = TIM3_IRQn;
							break;}
		case 15U:	{	TIM=TIM15; 
							RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;
							TIM_IRQn = TIM15_IRQn;
							break;}
		case 16U:	{	TIM=TIM16; 
							RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;
							TIM_IRQn = TIM16_IRQn;
							break;}
		default : {return;}
	}
	TIM->PSC = psc & 0x0000FFFFU;
	TIM->ARR = arr;
	TIM->CR1 |= TIM_CR1_ARPE | TIM_CR1_URS;
	if(irqUsed){
		TIM->DIER |= TIM_DIER_UIE;
		NVIC_EnableIRQ(TIM_IRQn); 
	}
	TIM->CR1 |= TIM_CR1_CEN;
}
