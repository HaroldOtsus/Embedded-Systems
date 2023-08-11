// In this lab I had to make a struct for a Moore finite state machine to create a traffic light.

// Runs on LM4F120/TM4C123
// Index implementation of a Moore finite state machine to operate a traffic light.  
// Daniel Valvano, Jonathan Valvano
// January 15, 2016

// east/west red light connected to PB5
// east/west yellow light connected to PB4
// east/west green light connected to PB3
// north/south facing red light connected to PB2
// north/south facing yellow light connected to PB1
// north/south facing green light connected to PB0
// pedestrian detector connected to PE2 (1=pedestrian present)
// north/south car detector connected to PE1 (1=car present)
// east/west car detector connected to PE0 (1=car present)
// "walk" light connected to PF3 (built-in green LED)
// "don't walk" light connected to PF1 (built-in red LED)

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// ***** 2. Global Declarations Section *****

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup            
  NVIC_ST_CTRL_R = 0x00000005;          // enable SysTick with core clock
}

void SysTick_Wait(unsigned int delay){
  NVIC_ST_RELOAD_R = delay-1;  // number of counts to wait
  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
	
  while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag
  }
}

void SysTick_Wait1ms(unsigned int delay){
  unsigned int i;
	
  for(i=0; i<delay; i++){
    SysTick_Wait(80000);  // wait 1ms
  }
}

struct State {

  unsigned int OutB;
	
	unsigned int OutF;

  unsigned int Time; 

  unsigned int Next[10];};

typedef const struct State State_t;
	
#define goSouth   0

#define waitSouth 1

#define goWest   2

#define waitWest 3
	
#define goWalk   4

#define HurryWalk1 5

#define HurryWalk2 6

#define HurryWalk3 7
	
#define HurryWalk4 8
	
#define waitWalk   9

State_t FSM[10]={

 {0x21, 0x02, 750, {goSouth, waitSouth, goSouth, waitSouth, waitSouth, waitSouth, waitSouth, waitSouth}},

 {0x22, 0x02, 500,{goWest, goWest, goSouth, goWest, goWalk, goWalk, goWalk, goWest}},

 {0x0C, 0x02, 750,{goWest, goWest, waitWest, waitWest, waitWest, waitWest, waitWest, waitWest}},

 {0x14, 0x02, 500,{goWalk, goWest, goSouth, goSouth, goWalk, goWalk, goSouth, goWalk}},
 
 {0x24, 0x08, 750,{goWalk, HurryWalk1, HurryWalk1, HurryWalk1, goWalk, HurryWalk1, HurryWalk1, HurryWalk1}},

 {0x24, 0x02, 500,{HurryWalk2, HurryWalk2, HurryWalk2, HurryWalk2, HurryWalk2, HurryWalk2, HurryWalk2, HurryWalk2}},

 {0x24, 0x00, 500,{HurryWalk3, HurryWalk3, HurryWalk3, HurryWalk3, HurryWalk3, HurryWalk3, HurryWalk3, HurryWalk3}},

 {0x24, 0x02, 500,{HurryWalk4, HurryWalk4, HurryWalk4, HurryWalk4, HurryWalk4, HurryWalk4, HurryWalk4, HurryWalk4}},
 
 {0x24, 0x00, 500,{waitWalk, waitWalk, waitWalk, waitWalk, waitWalk, waitWalk, waitWalk, waitWalk}},

 {0x24, 0x02, 500,{goSouth, goWest, goSouth, goWest, goWalk, goWest, goSouth, goSouth}}};

 unsigned int State;
 unsigned int Input;
 
// ***** 3. Subroutines Section *****

int main(void){ 
	volatile unsigned int delay;
  TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210,ScopeOff); // activate grader and set system clock to 80 MHz
  EnableInterrupts();
	SysTick_Init();
	SYSCTL_RCGC2_R = 0x32; 				//B, F, E clock
	delay = SYSCTL_RCGCGPIO_R;
	GPIO_PORTE_DIR_R = 0x00;			//Inputs on PE0-2
	GPIO_PORTE_DEN_R = 0x07;			//Digital on PE0-2
	GPIO_PORTB_DIR_R = 0x3F;			//Outputs on PB0-5
	GPIO_PORTB_DEN_R = 0x3F;			//Digital on PB0-5
	GPIO_PORTF_DIR_R = 0x0A;			//Outputs on PF1 and PF3
	GPIO_PORTF_DEN_R = 0x0A;			//Digital on PF1 and PF3
	State = goSouth;
  while(1){
    GPIO_PORTB_DATA_R = FSM[State].OutB;
		GPIO_PORTF_DATA_R = FSM[State].OutF;
		SysTick_Wait1ms(FSM[State].Time);
		Input = GPIO_PORTE_DATA_R;
		State = FSM[State].Next[Input];
  }
}

