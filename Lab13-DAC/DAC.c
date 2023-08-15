// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// Implementation of the 4-bit digital to analog converter
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// Port B bits 3-0 have the 4-bit DAC

#include "DAC.h"
#include "..//tm4c123gh6pm.h"

// **************DAC_Init*********************
// Initialize 4-bit DAC 
// Input: none
// Output: none
void DAC_Init(void){unsigned int volatile delay;
	SYSCTL_RCGCGPIO_R |= 0x02;					//Activate Port B
	delay = SYSCTL_RCGCGPIO_R;					//Allow time for Port B clock activation
	GPIO_PORTB_AMSEL_R &= ~0x0F;				//No analog
	GPIO_PORTB_PCTL_R &= ~0x00000FFF;		//Regular function
	GPIO_PORTB_DIR_R |= 0x0F;						//Make PB0-PB3 outputs
	GPIO_PORTB_AFSEL_R &= ~0x0F;				//Disable alt function
	GPIO_PORTB_DEN_R |= 0x0F;						//Enable digital on PB0-PB3
	GPIO_PORTB_DR8R_R |= 0x0F; 					//Can drive up to 8mA out though PB3-0
}


// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(unsigned long data){
  GPIO_PORTB_DATA_R = data;		//Output DAC values
}
