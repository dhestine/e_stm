#include "uart_driver.h"
#include "systick_config.h"

uint8_t uart1_success = 0; // for debugging

// stores the status of UART1 operations
UART_Status_t UART1_TX;

int main(void){

    //Enabling SysTick timer
	SysTick_Init();

	// UART1 configuration structure
	UART_Config_t uart1_config = {
			.baudrate = 115200,
			.parity = UART_PARITY_NONE,
			.stop_bits = UART_STOPBITS_1,
			.word_length = UART_WORDLENGTH_8B
	};

    // UART2 configuration structure
	UART_Config_t uart2_config = {
			.baudrate = 115200,
			.word_length = UART_WORDLENGTH_8B,
			.stop_bits = UART_STOPBITS_1,
			.parity = UART_PARITY_NONE,
	};

	// initialize UART1 and enable flag if OK
	if (UART1_Init(&uart1_config) == UART_OK){
		uart1_success = 1;
	}

	// Initialize UART2 and move forward if successful
	if (UART2_Init(&uart2_config) == UART_OK) {

		// Send a test message on UART2
		uint8_t msg[] = "Hello, UART2!\r\n";
		UART2_Transmit(msg, sizeof(msg) - 1, 1000);

		// To store each received byte from UART2
		uint8_t received_byte;

		// infinite loop
		while (1) {

			// To check if UART2 has received data
			if (UART2_IsDataAvailable()) {

				// read one byte received from UART2
				received_byte = UART2_ReceiveByte();

				// Send received byte to UART1 and store the status
				UART1_TX = UART1_TransmitByte(received_byte);

				// Echo UART1 TX result
				UART2_TransmitByte((uint8_t)UART1_TX);
			}
		}
	}

}
