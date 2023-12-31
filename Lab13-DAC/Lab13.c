// In this lab I made a piano with 4 keys that each played a different sound based on what button was pressed on the breadboard.

// Runs on LM4F120 or TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// edX Lab 13 
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// Port B bits 3-0 have the 4-bit DAC
// Port E bits 3-0 have 4 piano keys

#include "..//tm4c123gh6pm.h"
#include "Sound.h"
#include "Piano.h"
#include "TExaS.h"

unsigned long SW;

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void delay(unsigned long msec);
int main(void){ // Real Lab13 
	// for the real board grader to work 
	// you must connect PD3 to your DAC output
  TExaS_Init(SW_PIN_PE3210, DAC_PIN_PB3210,ScopeOn); // activate grader and set system clock to 80 MHz
// PortE used for piano keys, PortB used for DAC        
  Sound_Init(); // initialize SysTick timer and DAC
  Piano_Init();
  EnableInterrupts();  // enable after all initialization are done
  while(1){ 		
// input from keys to select tone
		SW = Piano_In();
		if(SW == 0x01)
			Sound_Tone(9556);			//(80MHz / 16 * 523.251Hz) / 2 (Piano key C)
		else if(SW == 0x02)
			Sound_Tone(8516);			//(80MHz / 16 * 587.330Hz) / 2 (Piano key D)
		else if(SW == 0x04)
			Sound_Tone(7584);			//(80MHz / 16 * 659.255Hz) / 2 (Piano key E)
		else if(SW == 0x08)
			Sound_Tone(6380);			//(80MHz / 16 * 783.991Hz) / 2 (Piano key G)
		else
			Sound_Off();
		
		//delay(10);
  }         
}

// Inputs: Number of msec to delay
// Outputs: None
void delay(unsigned long msec){ 
  unsigned long count;
  while(msec > 0 ) {  // repeat while there are still delay
    count = 16000;    // about 1ms
    while (count > 0) { 
      count--;
    } // This while loop takes approximately 3 cycles
    msec--;
  }
}


