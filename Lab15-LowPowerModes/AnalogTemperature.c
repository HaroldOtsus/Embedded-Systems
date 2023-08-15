/*
**	AnalogTemperature.c
**	This file contains initialisation of Analog-digital converter with internal CPU temperature sensor
**  activation. Also file contains reading temperature from ADC buffer and converting analog value
**	to Celsius.
*/

/* IMPORTS */
#include <stdio.h>
#include "AnalogTemperature.h"
#include "tm4c123gh6pm.h"

/* FUNCTIONS */

/*
**	Initialisation of temperature sensor.
**
**  This function activate temperature sensor by initialisation of ADC0 with additional initialisation
**  of SS3(Sample Sequencer) with special setting of TS0 bit
*/
void Temp_Sensor_Init(void) { volatile unsigned long delay;
	/*ADC0 with SS3 initialization*/
	SYSCTL_RCGC0_R |= 0x00010000;			//Activate clock for ADC0
	SYSCTL_RCGC0_R &= ~0x00000300;		//Set the sample speed for ADC0 to 125k samples/sec
	delay = SYSCTL_RCGC0_R;
	delay = SYSCTL_RCGC0_R;
	ADC0_SSPRI_R = 0x0123;						//Sequencer 3 is highest priority
	ADC0_ACTSS_R &= ~0x0008;					//Disable sample sequencer 3
	ADC0_EMUX_R &= ~0xF000;						//Set sequencer 3 to software trigger
	ADC0_SSMUX3_R &= ~0x000F;					//Clear the first 4 bits
	ADC0_SSMUX3_R += 1; 							//Set the channel
	ADC0_SSCTL3_R = 0x000E;						//TS0, IE0 and END0 are on, D0 is off
	ADC0_IM_R &= ~0x0008;							//Disable SS3 interrupts
	ADC0_ACTSS_R |= 0x0008;						//Enable sample sequencer 3
}

/*
**	Read value from temperature sensor.
**
**  This function activate ADC0 converter and read value from temperature sensor
**  in the ADC FIFO buffer.
**  
**  Function returns 0-4095 analog value that represents the temperature of CPU
*/
unsigned long ADC0_In(void){  
	unsigned long result;
	ADC0_PSSI_R = 0x0008;							// 1) initiate SS3
	while(!(ADC0_RIS_R & 0x08));			// 2) wait for conversion done
	result = ADC0_SSFIFO3_R & 0xFFF;	// 3) read result
	ADC0_ISC_R = 0x0008;							// 4) acknowledge completion

	return result;
}
