 #include "stm32f4xx.h"
 #include "adc_config.h"

 uint16_t adc_val; // variable to store raw ADC value
 float voltage;   // calculated voltage from ADC reading
 float temp;     // calculated temperature in celsius

 int main(void){

 // Initialises PA4 as ADC
 adc_PA_init(4);

 // Sets PA5 as output (clock already enable in adc_PA_init)
 GPIOA->MODER &= ~GPIO_MODER_MODER5_Msk;
 GPIOA->MODER |= GPIO_MODER_MODER5_0;

 while(1){

	// read ADC value from PA4
    adc_val = adc_PA_read();

    //convert ADC to voltage
    voltage = 3.3 * adc_val/4095;

    //convert VOLTAGE to TEMPERATURE
    temp = (voltage-0.5)*100.0;

    // LED control on PA5
 if (adc_val < 2048){
    GPIOA->BSRR |= GPIO_BSRR_BR5; // Turn LED OFF
 } else{
    GPIOA->BSRR |= GPIO_BSRR_BS5; // Turn LED ON
 }

   for(int i = 0; i < 400000; i++); // delay for LED
 }
 }
