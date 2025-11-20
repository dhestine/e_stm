#include "stm32f4xx.h"

void delay_ms(uint32_t ms) {
    // This is a very basic, non-blocking delay
    volatile uint32_t i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 1000; j++); // Adjust loop count for desired delay on your specific MCU clock speed
    }
}

int main(void){

	// ENABLE THE CLOCK FOR GPIOA AND GPIOC
	// The RCC ( Reset and Clock Control ) must provide a clock to GPIO before use.
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;

	//CONFIGURE PC5, PC6, PC8, PC9 AS OUTPUTS
	GPIOC->MODER |= GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0 ;

	while(1) {

		//To check if input on PC10 is LOW
		if(!(GPIOC->IDR & GPIO_IDR_ID10)){

			//turn on LEDs SEQUENTIALLY WHEN BUTTON IS PRESSED
			GPIOC->BSRR |= GPIO_BSRR_BS5; // Turn ON LED at PC5
			delay_ms(100);
			GPIOC->BSRR |= GPIO_BSRR_BR5; // Turn OFF LED at PC5

			GPIOC->BSRR |= GPIO_BSRR_BS6; // Turn ON LED at PC6
			delay_ms(200);
			GPIOC->BSRR |= GPIO_BSRR_BR6; // Turn OFF LED at PC6

			GPIOC->BSRR |= GPIO_BSRR_BS8; // Turn ON  LED at PC8
			delay_ms(300);
			GPIOC->BSRR |= GPIO_BSRR_BR8; // Turn OFF LED at PC8

			GPIOC->BSRR |= GPIO_BSRR_BS9; // Turn ON LED at PC9
			delay_ms(400);
			GPIOC->BSRR |= GPIO_BSRR_BR9; // Turn OFF LED at PC9

		}else{

			// turn off LEDs IN REVERSE ORDER IF BUTTON IS NOT PRESSED

			GPIOC->BSRR |= GPIO_BSRR_BS9; // Turn ON LED at PC9
			delay_ms(100);
			GPIOC->BSRR |= GPIO_BSRR_BR9; // Turn OFF LED at PC9

			GPIOC->BSRR |= GPIO_BSRR_BS8; // Turn ON LED at PC8
			delay_ms(200);
			GPIOC->BSRR |= GPIO_BSRR_BR8; //Turn OFF LED at PC8

			GPIOC->BSRR |= GPIO_BSRR_BS6; //Turn ON LED at PC6
			delay_ms(300);
			GPIOC->BSRR |= GPIO_BSRR_BR6; //Turn OFF LED at PC6

			GPIOC->BSRR |= GPIO_BSRR_BS5; //Turn ON LED at PC5
			delay_ms(400);
			GPIOC->BSRR |= GPIO_BSRR_BR5; //Turn OFF LED at PC5
		}

		// TIMER DELAY BEFORE THE NEXT CYCLE
		delay_ms(200);
	}


}
