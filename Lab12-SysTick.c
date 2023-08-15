// In this lab I learned how to create/use SysTick interrupts.

// Runs on LM4F120/TM4C123
// Use SysTick interrupts to create a squarewave at 440Hz.  
// There is a positive logic switch connected to PA3, PB3, or PE3.
// There is an output on PA2, PB2, or PE2. The output is 
//   connected to headphones through a 1k resistor.
// The volume-limiting resistor can be any value from 680 to 2000 ohms
// The tone is initially off, when the switch goes from
// not touched to touched, the tone toggles on/off.
//                   |---------|               |---------|     
// Switch   ---------|         |---------------|         |------
//
//                    |-| |-| |-| |-| |-| |-| |-|
// Tone     ----------| |-| |-| |-| |-| |-| |-| |---------------
//
// Daniel Valvano, Jonathan Valvano
// January 15, 2016

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2015

 Copyright 2016 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */


#include "TExaS.h"
#include "..//tm4c123gh6pm.h"

unsigned long SW;
unsigned long Memory;
unsigned long Flag;

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);  // low power mode

// input from PA3, output from PA2, SysTick interrupts
void Sound_Init(void){ unsigned int volatile delay;
	SYSCTL_RCGCGPIO_R |= 0x00000001;		//Port A activate
	delay = SYSCTL_RCGCGPIO_R;
	GPIO_PORTA_DIR_R |= 0x04;						//Make PA2 output
	GPIO_PORTA_DIR_R &= ~0x08;					//Make PA3 input
	GPIO_PORTA_DR8R_R |= 0x04;					//Output can drive up to 8mA
	GPIO_PORTA_DEN_R |= 0x0C;						//Enable digital on PA2 and PA3
	NVIC_ST_CTRL_R = 0;									//Disable SysTick during setup
	NVIC_ST_RELOAD_R = 90908;						//Reload value for 1.13636ms ((1.13636ms / 12.5ns) - 1)
	NVIC_ST_CURRENT_R = 0;              //Clear current register
	NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R&0x00FFFFFF; //Priority 0
	NVIC_ST_CTRL_R = 0x00000007;  			//Enable with core clock and interrupts
	Memory = 0;
	Flag = 0;
}

// called at 880 Hz
void SysTick_Handler(void){
	if(Flag){												//If flag is high or not
		GPIO_PORTA_DATA_R ^= 0x04;		//Toggles PA2
	}
	else{
		GPIO_PORTA_DATA_R &= ~0x04;		//Clears PA2
	}
}

int main(void){// activate grader and set system clock to 80 MHz
  TExaS_Init(SW_PIN_PA3, HEADPHONE_PIN_PA2,ScopeOn); 
  Sound_Init();         
  EnableInterrupts();   // enable after all initialization are done
  while(1){
    // main program is free to perform other tasks
    // do not use WaitForInterrupt() here, it may cause the TExaS to crash
		SW = GPIO_PORTA_DATA_R&0x08;					//Read PA3 into SW
		
		if(SW){									//If switch is pressed
			if(Memory != SW){			//If the last input is not the same as current input
				Flag ^= 1;					//Toggle the flag value
				Memory = SW;				//Set the current input to Memory to check later
			}
		}
		else{										//If switch is not pressed
			if(Memory != SW){			//If the last input is not the same as current input
				Memory = SW;				//Set the current input to Memory to check later
			}
		}
  }
}
