#include "systick_config.h"

#define SYSTICK_LOAD_VALUE 16000-1 // SysTick reload value for a 1ms delay

void delayMs(int timeMs){ // delay function

	// Set the reload value for 1ms tick period
	SysTick->LOAD = SYSTICK_LOAD_VALUE;

	//Clear current value register
	SysTick->VAL = 0;

	// Set clock source to processor clock
	SysTick->CTRL |= (1 << SysTick_CTRL_CLKSOURCE_Pos);

	//  Enable counter and start counting down
	SysTick->CTRL |= (1 << SysTick_CTRL_ENABLE_Pos);

	// Delay loop: wait for exactly 1ms
	for(int i=0; i<timeMs;i++){

		//Wait until counter flag to be set and count down to zero
		while(!(SysTick->CTRL & (1 << SysTick_CTRL_COUNTFLAG_Pos))){

		}
	}

	// Turn off timer after delay completes
	SysTick->CTRL = 0;
}
