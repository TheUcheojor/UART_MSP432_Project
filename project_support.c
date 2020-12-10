#include "msp.h"
#include "project_header.h"

/*
	File project_support.c
	Purpose: This file provides functions that configures ports,interrupts, NVIC, and UART
					
	Author: Paul Okenne
	Credits: 
			William Goh
			Texas Instruments Inc
*/


/*
	Function configurePorts
	Purpose: This function configures PORT1 (Pin 0,1,&4) and
					PORT2(Pin 0)
*/
void configurePorts(void){
	
		//Set GPIO Function
		P1SEL0&=~(uint8_t)(1<<0|1<<1|1<<4);
		P1SEL1&=~(uint8_t)(1<<0|1<<1|1<<4);
	
		//Set Port 1 - Pin 1 & 4 as an input with a pull-up resistor
		P1DIR&=~(uint8_t)(1<<1|1<<4);
		P1REN|=(uint8_t)(1<<1|1<<4);
		P1OUT|=(uint8_t)(1<<1|1<<4);
	
		//Set Port 1 - Pin 0 as an output
		P1DIR|=(uint8_t)(1<<0);
	
		//Set Port 2 to the GPIO Port Function
		P2SEL0&=~(uint8_t)(1<<0);
		P2SEL1&=~(uint8_t)(1<<0);
	
		//Set Port 2 - Pin 0 as an output
		P2DIR|=(uint8_t)(1<<0);
}


/*
	Function configureInterrupts
	Purpose: This function configures the interrupts for Port 1 - Pin 1 &4

*/
void configureInterrupts(void){
	//Device interrupt configurations for Port 1 - Pin 1 & Pin 4	
	P1IE|=(uint8_t)(1<<1|1<<4);//Enable interrupts
	P1IES|=(uint8_t)(1<<1|1<<4);//Active low interrupts
	P1IFG&=~(uint8_t)(1<<1|1<<4);//Clear interrupt flag	
}


/*
	Function configureNVIC
	Purpose: Configure interrupts at the NVIC level for port 1 
*/
void configureNVIC(void){
	 //Configure NVIC for Port 1
    NVIC_SetPriority(PORT1_IRQn, 2);
    NVIC_ClearPendingIRQ(PORT1_IRQn);
    NVIC_EnableIRQ(PORT1_IRQn);
}

/*
	Function configureUART
	
*/
void configureUART(void){
	
		CS->KEY = CS_KEY_VAL;                   // Unlock CS module for register access
    CS->CTL0 = 0;                           // Reset tuning parameters
    CS->CTL0 = CS_CTL0_DCORSEL_3;           // Set DCO to 12MHz (nominal, center of 8-16MHz range)
    CS->CTL1 = CS_CTL1_SELA_2 |             // Select ACLK = REFO
            CS_CTL1_SELS_3 |                // SMCLK = DCO
            CS_CTL1_SELM_3;                 // MCLK = DCO
    CS->KEY = 0;                            // Lock CS module from unintended accesses

    // Configure UART pins
    P1->SEL0 |= BIT2 | BIT3;                // set 2-UART pin as secondary function

    // Configure UART
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST | // Remain eUSCI in reset
            EUSCI_B_CTLW0_SSEL__SMCLK;      // Configure eUSCI clock source for SMCLK
    // Baud Rate calculation
    // 12000000/(16*9600) = 78.125
    // Fractional portion = 0.125
    // User's Guide Table 21-4: UCBRSx = 0x10
    // UCBRFx = int ( (78.125-78)*16) = 2
    EUSCI_A0->BRW = 78;                     // 12000000/16/9600
    EUSCI_A0->MCTLW = (2 << EUSCI_A_MCTLW_BRF_OFS) |
            EUSCI_A_MCTLW_OS16;

    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; // Initialize eUSCI
    EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG;    // Clear eUSCI RX interrupt flag
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;        // Enable USCI_A0 RX interrupt

    // Enable sleep on exit from ISR
    //SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;

    // Enable global interrupt
    __enable_irq();

    // Enable eUSCIA0 interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((EUSCIA0_IRQn) & 31);
	
		// Enter LPM0
		__sleep();
    __no_operation();                       // For debugger
}
