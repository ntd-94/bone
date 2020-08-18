/*
 ============================================================================
 Name        : adc1115.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <termios.h>
#include <time.h>
#include "wiringPi.h"
#include "wiringPiI2C.h"
#include "ads1115.h"

int handle;

double readADS1115() {

	int adcReading;
	unsigned char buffer[10];
	double voltage;

	usleep(2000);
	read(handle, buffer, 2);
	adcReading = ((buffer[0] << 8) | buffer[1]);
	voltage = (double)adcReading * (double)0.000125;
	usleep(1000);
	return voltage;
}

int configADS1115() {

	unsigned char buffer[4];
	int status;
	uint16_t adcCfg=0;

	buffer[0] = ADS1015_REG_POINTER_CONFIG;
	adcCfg = ADS1015_REG_CONFIG_MUX_SINGLE_0 | ADS1015_REG_CONFIG_PGA_4_096V |
			ADS1115_REG_CONFIG_DR_16SPS;
	//Mode continuous

    printf("ADC Cfg %04x\n", adcCfg);
	buffer[1] = adcCfg >> 8;
	buffer[2] = adcCfg & 0x00FF;

	//Write to conversion register. This is a block write
	status = write(handle, buffer, 3);
	if (status == -1) {
		printf("Error writing block during initialization.\n");
		return status;
	}
	wiringPiI2CWrite(handle, ADS1015_REG_POINTER_CONVERT);
	usleep(2000);
	return 0;
}


int main(int argc, char *args[]) {
	int i, status;
	double vMeasure;


	handle = wiringPiI2CSetup(ADS1015_ADDRESS);
	if (handle < 0) {
		printf("Error number %d opening device.\n", handle);
		return -1;
	}
	status = configADS1115();
	if (status == -1) {
		printf("Error configuring device.\n");
		return -1;
	}

	for (i = 0; i < 50; i++) {
		vMeasure = readADS1115();
		printf("Voltage %.3f\n", vMeasure);
		fflush(stdout);
		usleep(100000);
	}
	puts("Done");
	close(handle);
	return EXIT_SUCCESS;
}

