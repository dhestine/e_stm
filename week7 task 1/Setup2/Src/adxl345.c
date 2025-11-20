#include "adxl345.h"

// Temporary variable to store single-byte reads
char data;

// Array to store 6 bytes of acceleration data (x, y, z)
uint8_t rec_data[6];

// Read one register from the ADXL345
void adxl_read_address(uint8_t reg){
    I2C1_readByte(DEVICE_ADDR, reg, &data);
}

// Write a single byte to ADXL345 register
void adxl_write(uint8_t reg, char value){

	char data[1];
	data[0] = value; // Store value in array

	I2C1_burstWrite(DEVICE_ADDR, reg, 1, data);

}

// Reads 6 consecutive data registers from ADXL345
void adxl_read_values(uint8_t reg){

	I2C1_burstRead(DEVICE_ADDR, reg, 6, (char*)rec_data);

}

// Set up the ADXL345 accelerometer so it is ready to take measurement
void adxl_init(void){

	I2C1_init(); // Initialize 12C1 peripheral

	adxl_read_address(DEVID); // Read the device ID

	adxl_write(DATA_FORMAT, FOUR_G); // Set the accelerometer range

	adxl_write(PWR_CTL, RESET); // Reset the power control register

	adxl_write(PWR_CTL, SET_MEASURE_B); // Put the accelerometer into measurement mode so it start giving data

}
