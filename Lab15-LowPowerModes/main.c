// In this lab I experimented with different types of low power modes that were available on microcontrollers and measured the temperature rating when on each type of mode.

/*
** main.c
** 
** - Runs on TM4C123
** - UART runs at 115,200 baud rate 
**
*/

/* IMPORTS */

#include "tm4c123gh6pm.h"
#include <stdio.h>
#include "uart.h"
#include "AnalogTemperature.h"
#include "SysTick.h"
#include "Hibernate.h"

/* PROTOTYPES */
void WaitForInterrupt(void);  // Sleep-mode activation

/* FUNCTIONS */

/*
**	Delay function
**
**  Function simulate delay of the microcontroller
**
**  Argument ms represents the unsigned long number of milliseconds needed to be delayed
**
**  NOTE: Function assumes, that the speed of main oscillator is 16MHz
*/
void Delay(unsigned long ms){unsigned long time;
  time = 2667 * ms; // 1ms
  while(time){
   time--;
  }
}

/*
** Initialisation of the PF3 and PF1 ports of the Port F
**
*/
void PortF_Init(void){ volatile unsigned long delay;
	/* Port F initialization */
	SYSCTL_RCGC2_R |= 0x00000020;      //F clock
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTF_AMSEL_R &= ~0x0A;       //Disable analog function
	GPIO_PORTF_PCTL_R &= ~0x0A;   		 //GPIO clear bit PCTL
	GPIO_PORTF_DIR_R |= 0x0A;          //PF1 and PF3 outputs
	GPIO_PORTF_AFSEL_R &= ~0x0A;       //No alternate function
	GPIO_PORTF_DEN_R |= 0x0A;          //Enable digital pins PF1 and PF3
}

/*
**	Convert analog value to Celsius
**
**  Function use expression from Tiva C Series TM4C123GH6PM datasheet (p. 813) 
**  
**	Argument sample represents the analog sample, that has been recieved 
**  from ADC0_In() functions
**
**  Function returns float value, that represents temperature of CPU in Celsius
*/
float Convert_To_Cel (unsigned long sample) {
	return (147.5 - ((75 * 3.3 * sample) / 4096));
}


/*
**	Print temperature in Celsius on TeXaS display
**
*/
void Print_Temp() {
	unsigned long result = 0;
	float celsius = 0.0f;
	
	/*Read analog value from temperature sensor*/
	GPIO_PORTF_DATA_R |= 0x02;		//Red LED on
	result = ADC0_In();
	GPIO_PORTF_DATA_R &= ~0x02;		//Red LED off
	/*Convert analog value in Celsius degrees format*/
	GPIO_PORTF_DATA_R |= 0x08;		//Green LED on
	celsius = Convert_To_Cel(result);
	GPIO_PORTF_DATA_R &= ~0x08;		//Green LED off
	
	printf("Temperature in C: %.1f\n", celsius);
}

/*
**	NVIC interrupt handler
**
**	Function print CPU Temperature in the debbug console every 5 seconds
**
*/
void SysTick_Handler(void){ 
	Print_Temp();                   // Print Temperature
}

/*
** Main function, that have run mode active
*/
int main (void) {
	UART_Init();    		// initialize UART for printing
	PortF_Init();   		// initialize Port F for Green and Red LED
	Temp_Sensor_Init();		// initialize temperature sensort
	printf("\nStarting active mode programm\n");
	while (1) {
		Delay(2000);								// Simulate 2 seconds delay
		Print_Temp();								// Print temperature
	}
}

/*
** Main function, that have sleep mode active
*/
int main2 (void) {
	UART_Init();    					// initialize UART for printing
	PortF_Init();   					// initialize Port F for Green and Red LED
	Temp_Sensor_Init();  			// initialize temperature sensort
	SysTick_Init(7999999);  	// initialize  NVIC interrupts for sleep mode
	printf("\nStarting sleep mode programm\n");
	WaitForInterrupt();       // Put microcontroller in sleep mode
	return 0;
}

/*
** Main function, that have hibernation mode active
*/
int main3 (void) {
	UART_Init();            	// initialize UART for printing
	PortF_Init();           	// initialize Port F for Green and Red LED
	Hibernate_Mode_Init();  	// initialize hibernation mode
	Temp_Sensor_Init();     	// initialize temperature sensort
	
	if (!(HIB_RIS_R&0x01)) printf("\nStarting hibernation mode programm\n");  // Check for Hibernation interrupt bit
	else printf("\nWaking up from hibernation\n");
	
	Delay(25);                  // Give 25ms delay for microcontroller to stabilize system after wakining up
	Print_Temp();								// Print temperature
	
	GPIO_PORTF_DATA_R |= 0x08;  // Turn on LED to acknowledge, that microcontroller is currently active
	Delay(5000);								// Simulate 5 seconds delay
	
	GPIO_PORTF_DATA_R &= ~0x08; // Turn off LED after 5 seconds delay
	Activate_Hibernate();				// Go to hibernation
	while (1) {	
	}
}
