/*
**	Hibernate.c
**	This file have basic initialisation and activation
**	of TM4C123GH6PM hibernation mode.
**	
**
**	!!!IMPORTANT: TM4C123GH6PM has an external 32.768-kHz oscillator for RTC timer and
**	hibernation mode, that is seperated from the main oscillator. Because of that every writting
**  to the hibernation registers has to be followed with polling WRC(Write Complete/Capable) bit
**  in HIBCTL(Hibernation Control) register. Therefore in Hibernate.h is made special macro
**  "ChangeHIBRegSettings(x)" where instead of x should be written initial writting to the
**  hibernation register. This macro is handling WRC bit polling by itselfs.
*/

/* IMPORTS */
#include "Hibernate.h"
#include "tm4c123gh6pm.h"


/* FUNCTIONS */

/* Hibernation mode initialisation */
void Hibernate_Mode_Init(void) { volatile unsigned long delay;
	SYSCTL_RCGCHIB_R |= SYSCTL_RCGCHIB_R0; //Enable clock for the hibernation mode
	delay = SYSCTL_RCGCHIB_R;              //Wait for clock stabilization
			
	ChangeHIBRegSettings(HIB_CTL_R |= 0x40);		//Enable 32.768-kHz Hibernation oscillator.
}

/* 
** Reset RTC timer and set new value to the RTC match timer 
**
** Argument seconds represent the unsigned long number of seconds, that has to be counted by the RTC to start
** wakinig up process of the microcontroller
**
*/
void SetRTCTime(unsigned long seconds) {		
	ChangeHIBRegSettings(HIB_RTCLD_R = 0);				//Reset the RTC timer by loading 0 value to the RTC load register
		
	ChangeHIBRegSettings(HIB_RTCM0_R = seconds);		//Set the custom value to the RTC mathc register
}

/* Function activate hibernation mode of microcontroller */
void Activate_Hibernate (void) {
 ChangeHIBRegSettings(SetRTCTime(2));		//Set a 2-second hibernate timer for waking up microcontroller after 2 seconds
	
 // Write settings to HIBCTL register
 // to activate hibernation mode
	ChangeHIBRegSettings(HIB_CTL_R |= 0x0000010B);
}
