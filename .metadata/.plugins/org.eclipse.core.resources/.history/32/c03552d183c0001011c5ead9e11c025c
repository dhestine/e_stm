/*
 * This is a setup project file including the CMSIS API
 * A baremetal code to blink the onboard LED (PA5) in the
 * Nucleo-F411RE board is provided below.
 */

#include "stm32f4xx.h"

int main(void){

	// Enable clock to GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	// Set GPIOA PIN 5 as output
	GPIOA->MODER &= ~(GPIO_MODER_MODER5_Msk);
	GPIOA->MODER |= GPIO_MODER_MODER5_0;

	//Infinite loop
	while(1){
		GPIOA->ODR ^= GPIO_ODR_OD5; // Toggles the LED
		for(int i = 0; i < 200000; i++);
	}
}
