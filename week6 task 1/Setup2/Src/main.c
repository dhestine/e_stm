#include "exti.h"
#include "myClockConf.h"
#include "TimerConf.h"
#include "stm32f4xx.h"

// This is the bit in the EXTI register that tells us if PC13 caused an interrupt
#define UBTN EXTI_PR_PR13

uint32_t duty = 0; // this will store the PWM duty cycle

int main(void){

	setClock(); // configure system clock
	pc13_exti_init(); // Set up PC13 as an external interrupt
	Timer2_PWM_Init(); // Set up Timer 2 so it can generate a PWM signal

    // Main loop
	while(1){}


}


void EXTI15_10_IRQHandler(void){

	// Check if interrupt was triggered by PC13
	if(EXTI->PR & UBTN){
		duty += 10; // Increment duty cycle by 10% each button press

		// if the duty cycle goes above 100%, start again at 0%
		if(duty > 100){
			duty = 0;
		}

		// Update Timer 2 PWM output with new duty cycle
		Timer2_SetDutyCycle(duty);
	}

	// Clear the interrupt pending flag for PC13 by writing 1
	EXTI->PR |= UBTN;
}
