/*
**	SysTick.c
**	This file have basic initialisation of NVIC with interrupt handling function
*/

/* IMPORTS */
#include "SysTick.h"
#include "tm4c123gh6pm.h"

/* FUNCTIONS */

/*
**	NVIC initialisation
**
**	To initialise NVIC, single element should be pass to this function.
** 
**  Argument period represents 24-bit reload value of NVIC system, 0x000000 - 0xFFFFFF
**
*/
void SysTick_Init(unsigned long period){
	/*All needed SysTick setting*/
	NVIC_ST_CTRL_R = 0;						//Disable SysTick during setup
	NVIC_ST_RELOAD_R = period;		//Reload value
	NVIC_ST_CURRENT_R = 0;      	//Any write to current clears it
	NVIC_SYS_CTRL_R |= 0x02;			//Activate SLEEPEXIT bit
	NVIC_ST_CTRL_R = 0x00000003;	//Enable SysTick and interrupts
}
