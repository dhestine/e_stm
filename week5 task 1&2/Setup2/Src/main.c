#include "stm32f4xx.h"

// Global variables for period measurement
volatile uint32_t tim3_overflow = 0; // Counter for timer overflows
volatile uint32_t last_capture = 0; // Stores previous capture time stamp
volatile uint32_t period_ticks = 0; // Number of timer tick between two captures
volatile float period_ms = 0; // Calculate period in milliseconds

void TIM3_IC_callback(void);

// Initialize TIM3 CH4 Input Capture on PC9
void inputElement_TIM3CHA4(void)

    // Enable Clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // Enable GPIOC clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; // Enable TIM3 clock

    // Set PC9 as Alternate Function AF2 (TIM3_CH4)
    GPIOC->MODER &= ~GPIO_MODER_MODER9;
    GPIOC->MODER |= GPIO_MODER_MODER9_1;
    GPIOC->AFR[1] &= ~GPIO_AFRH_AFSEL9;
    GPIOC->AFR[1] |= (2 << GPIO_AFRH_AFSEL9_Pos);

    // Configure TIM3 Timer
    TIM3->PSC = 1 - 1;
    TIM3->ARR = 0xFFFF;

    // Input capture on CH4
    TIM3->CCMR2 &= ~TIM_CCMR2_CC4S;
    TIM3->CCMR2 |= TIM_CCMR2_CC4S_0;

    // Capture on rising edge
    TIM3->CCER &= ~TIM_CCER_CC4P;
    TIM3->CCER &= ~TIM_CCER_CC4NP;

    TIM3->CCER |= TIM_CCER_CC4E; // Enable capture on CH4

    // Enable interrupts
    TIM3->DIER |= TIM_DIER_UIE | TIM_DIER_CC4IE;

    TIM3->EGR |= TIM_EGR_UG; // Generate an update event to load PSC and ARR values
    TIM3->CR1 |= TIM_CR1_CEN; // Start the timer by enabling the counter

    // Enable TIM3 interrupt in  NVIC
    NVIC_EnableIRQ(TIM3_IRQn);
}

int main(void) {

	// Initialize TIM3 input capture on PC9
    inputElement_TIM3CHA4();

    // infinite loop ( CPU  waits for interrupts )
    while (1){

    }
}

// TIM3 interrupt handler
void TIM3_IRQHandler(void) {

    // handle timer overflow
    if (TIM3->SR & TIM_SR_UIF) {

        tim3_overflow++; // Increment overflow counter to extend measurement range
        TIM3->SR &= ~TIM_SR_UIF; // Clear the update interrupt flag
  }

    // Handle input capture event
    if (TIM3->SR & TIM_SR_CC4IF) {

        TIM3_IC_callback(); // Call the callback function to process the capture
        TIM3->SR &= ~TIM_SR_CC4IF; // Clear the capture interrupt flag
    }
}

// CallbacK executed when a capture occurs on TIM3 CH4
void TIM3_IC_callback(void){

	// Read the current capture timer value
    uint32_t capture = TIM3->CCR4;

    // Combine overflow count with timer value
    uint32_t timestamp = (tim3_overflow << 16) | capture;

    // Calculate period a timer ticks ( difference between time stamps)
    period_ticks = timestamp - last_capture;

    // Store current time stamp for next calculation
    last_capture = timestamp;


    period_ms = period_ticks / 16000.0f; // Convert ticks to millisecond
}
