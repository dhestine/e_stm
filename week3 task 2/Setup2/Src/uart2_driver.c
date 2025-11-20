#include "uart_driver.h"
#include "stm32f4xx.h"
#include <stdbool.h>
#include "systick_config.h"

// Timeout value (in ms) for a single operation
#define UART2_TIMEOUT_MS 1000

// ====================== UART DRIVER ======================

// GPIO Configuration for USART2
// PA2 - USART2_TX (AF7)
// PA3 - USART2_RX (AF7)

UART_Status_t UART2_Init(UART_Config_t* config) {
    // Enable SysTick if not already running

    // Enable clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;   // Enable GPIOA clock
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;  // Enable USART2 clock

    // Configure GPIO pins PA2 (TX) and PA3 (RX)
    GPIOA->MODER &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3);
    GPIOA->MODER |= (GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1);

    // set high speed for TX/RX pins
    GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR2 | GPIO_OSPEEDER_OSPEEDR3);

    // Configure no pull for TX, pull-up for RX
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR2 | GPIO_PUPDR_PUPDR3);
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR3_0;

    // Select AF7 for PA2 and PA3
    GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL2 | GPIO_AFRL_AFRL3);
    GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL2_Pos) | (7 << GPIO_AFRL_AFSEL3_Pos);

    // Reset and Configure USART2
    USART2->CR1 = 0; // reset control register 1
    USART2->CR2 = 0; // reset control register 2
    USART2->CR3 = 0; // reset control register 3

    // Configure USART2
    USART2->CR1 |= config->word_length | config->parity;
    USART2->CR2 |= config->stop_bits;

    // Enable baud rate using
    if (UART2_SetBaudRate(config->baudrate) != UART_OK) {
        return UART_ERROR;
    }

    // Enable USART2, transmitter and receiver
    USART2->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;

    return UART_OK;
}


UART_Status_t UART2_DeInit(void) {
    USART2->CR1 &= ~USART_CR1_UE; // disable USART2
    RCC->APB1ENR &= ~RCC_APB1ENR_USART2EN; // disable clock
    GPIOA->MODER &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3); // return pin to input state
    return UART_OK;
}


UART_Status_t UART2_SetBaudRate(uint32_t baudrate) {
    uint32_t pclk = 16000000; // APB1 at 16 MHz set default
    uint32_t tmp  = (pclk + baudrate/2) / baudrate;

    if (tmp < 16 || tmp > 0xFFFF) {
        return UART_ERROR;
    }
    USART2->BRR = tmp; //load baud rate
    return UART_OK;
}


UART_Status_t UART2_Transmit(uint8_t* data, uint16_t size, uint32_t timeout) {
    uint32_t start_tick = GetTick(); // recording starting time

    for (uint16_t i = 0; i < size; i++) {

    	// waiting until TXE flag is set
        while (!(USART2->SR & USART_SR_TXE)) {
            if ((GetTick() - start_tick) > timeout) {
                return UART_TIMEOUT;
            }
        }
        USART2->DR = data[i];
    }

    // wait for final transmission to complete
    while (!(USART2->SR & USART_SR_TC)) {
        if ((GetTick() - start_tick) > timeout) {
            return UART_TIMEOUT;
        }
    }

    return UART_OK;
}


UART_Status_t UART2_Receive(uint8_t* data, uint16_t size, uint32_t timeout) {
    uint32_t start_tick = GetTick();

    for (uint16_t i = 0; i < size; i++) {

    	// wait until RXNE flag is set
        while (!(USART2->SR & USART_SR_RXNE)) {
            if ((GetTick() - start_tick) > timeout) {
                return UART_TIMEOUT;
            }
        }

        // Read received byte
        data[i] = (uint8_t)(USART2->DR & 0xFF);
    }
    return UART_OK;
}


UART_Status_t UART2_TransmitByte(uint8_t data) {
    uint32_t start_tick = GetTick();

    // waiting for TX
    while (!(USART2->SR & USART_SR_TXE)) {
        if ((GetTick() - start_tick) > UART2_TIMEOUT_MS) {
            return UART_TIMEOUT;
        }
    }
    USART2->DR = data; // send byte
    return UART_OK;
}


uint8_t UART2_ReceiveByte(void) {

	//blocking wait until a byte is received
    while (!(USART2->SR & USART_SR_RXNE));
    return (uint8_t)(USART2->DR & 0xFF);
}

  // To check IF received data is available
bool UART2_IsDataAvailable(void) {
    return (USART2->SR & USART_SR_RXNE) != 0;
}

  // To check IF transmission is complete
bool UART2_IsTransmitComplete(void) {
    return (USART2->SR & USART_SR_TC) != 0;
}
