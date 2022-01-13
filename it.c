#include "it.h"

void TIM2_IRQHandler(){
	if(TIM2->SR & TIM_SR_UIF){
		TIM2->SR &= ~TIM_SR_UIF;
		
	}
}

void TIM3_IRQHandler(){
	if(TIM3->SR & TIM_SR_UIF){
		TIM3->SR &= ~TIM_SR_UIF;
		
	}
}

void TIM15_IRQHandler(){
	if(TIM15->SR & TIM_SR_UIF){
		TIM15->SR &= ~TIM_SR_UIF;
		
	}
}

void TIM16_IRQHandler(){
	if(TIM16->SR & TIM_SR_UIF){
		TIM16->SR &= ~TIM_SR_UIF;
		
	}
}
