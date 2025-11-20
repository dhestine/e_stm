#include "stm32f4xx.h"
#include <stdio.h>
#include <stdint.h>
#include "adxl345.h"

// Variable to store raw acceleration values from the sensor
int16_t x,y,z;

// Convert acceleration values in units of 'g'
float xg, yg, zg;

// This array is filled with 6 bytes of data read from the ADXL345
extern uint8_t rec_data[6];

int main(void) {

 adxl_init(); //Initialize ADXL345 accelerometer

while(1){

 // Read 6 bytes of data starting from the first data register
 adxl_read_values(DATA_START_ADD);

 // The ADXL345 Combine high and low bytes into signed integers for each axis
 x = ((rec_data[1] << 8) | rec_data[0]); // X-axis value
 y = ((rec_data[3] << 8) | rec_data[2]); // Y-axis value
 z = ((rec_data[5] << 8) | rec_data[4]); // Z-axis value

 // Convert raw LSB values to 'g' units
 xg = x * 0.0078f; // convert X-axis to g units
 yg = y * 0.0078f; // Convert Y-axis to g units
 zg = z * 0.0078f; // Convert Z-axis to g units

   }

 }
