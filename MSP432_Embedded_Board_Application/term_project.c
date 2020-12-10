/*
	File term_project.c
	
	Purpose:	The file implements a simple state machine wth two LEDs.
						The state can be incremented or decremented with butttons and 
						communications over a serial port
	
	Author:Paul Okenne
*/

#include "msp.h"
#include "project_header.h"

int state=0;

/*
	Function changeState
	Purpose: This function changes the current state and
					 sets the output to the corressponding state
					 
	Parmeter: incrementValue:int - the value to be added to the current state 

*/
void changeState(int stateIncrement){
	
	//Set the new state
	state+=stateIncrement;
	if(state>3)  state=0;
	if(state<0) state=3;
	
	if(state==0){//Check if state is 0
			
			//Set Button 1 and Button 2 as OFF
			P1->OUT &= (uint8_t)(~((BIT0)));
			P2->OUT &= (uint8_t)(~((BIT0)));
		
			EUSCI_A0->TXBUF='0';//Output 0 to the TX pin
		
	}else if(state==1){//Check if state is 1
		
			//Set Button 1 as ON and Button 2 as OFF
			P1->OUT |= (uint8_t)(((BIT0)));
			P2->OUT &= (uint8_t)(~((BIT0)));
		
			EUSCI_A0->TXBUF='1';//Output 1 to the TX pin
		
	}else if(state==2){//Check if state is 2
		
			//Set Button 1 as OFF and Button 2 as ON
			P1->OUT &= (uint8_t)(~((BIT0)));
			P2->OUT |= (uint8_t)(((BIT0)));
		
			EUSCI_A0->TXBUF='2';//Output 2 to the TX pin
		
	}else if(state==3){//Check if state is 3
		
			//Set Button 1 and Button 2 as ON
			P1->OUT |= (uint8_t)(((BIT0)));
			P2->OUT |= (uint8_t)(((BIT0)));
		
			EUSCI_A0->TXBUF='3';//Output 3 to the TX pin
	}
	
}



int main(void){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // Stop watchdog timer
	
		configurePorts();
		configureInterrupts();
		configureNVIC();
		configureUART();
	
		//Enable Global Interrupts
		__ASM("CPSIE I");

		while(1){}
}

/*
	Function PORT1_IRQHandler
	Purpose: Increment or decrement state when button 1 or 2 is pressed
	Port1 Interrupt Service Routine
*/
void PORT1_IRQHandler(void){
		
	if( P1IFG & (uint8_t)(1<<1) ){//Check if Button 1 is pressed
			P1IFG&=~(uint8_t)(1<<1);//Clear the interrupt flag			
			changeState(+1);//Increment the state by 1
		
	}else if( P1IFG & (uint8_t)(1<<4) ){//Check if Button 2 is pressed
			P1IFG&=~(uint8_t)(1<<4);//Clear the interrupt flag	
			changeState(-1);//Decrement the state
	}
	
}

/*
	Function EUSCIA0_IRQHandler
	Purpose: Increment or decrement state when 'w' or 's' is innputed into the TX pin
	UART interrupt service routine
*/
void EUSCIA0_IRQHandler(void)
{
			
   if (EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG)
    {
				volatile  uint8_t receivedValue;   // volatile currentValue register to avoid optimizing
			
			  // read RXBUF 
				receivedValue = EUSCI_A0->RXBUF;

        // Check if the TX buffer is empty first
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
				
			
				if(receivedValue == 'w'){//Check if the received value is 'w'
					changeState(+1);//increment state
				}else if(receivedValue=='s'){//Check if the received buffer value is 's'
					changeState(-1);//decrement state
				}		
				
    }
}