#include "systick_config.h"

#define SYSTICK_LOAD_VALUE 16000-1 //in a ms

void delayMs(int timeMs){
	//Reload with number of clocks per millisecond
	SysTick->LOAD |= SYSTICK_LOAD_VALUE;
	//Clear sysTick current value register
	SysTick->VAL = 0;
	//ENable Timer SysTick
	SysTick->CTRL |= (1 << SysTick_CTRL_CLKSOURCE_Pos);
	SysTick->CTRL |= (1 << SysTick_CTRL_ENABLE_Pos);

	for(int i=0; i<timeMs;i++){
		//Wait until counter flag is set
		while(!(SysTick->CTRL & (1 << SysTick_CTRL_COUNTFLAG_Pos))){}
	}
	SysTick->CTRL = 0;
}
