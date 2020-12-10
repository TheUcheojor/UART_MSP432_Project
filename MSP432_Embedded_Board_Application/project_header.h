#ifndef TERM_PROJECT_HEADER_H
#define TERM_PROJECT_HEADER_H

void configurePorts(void);//configures PORT1 (Pin 0,1,& 4) and PORT2(Pin 0)
void configureInterrupts(void);//configure interrupts for  PORT1 (Pin 1,& 4)
void configureNVIC(void);//configure interrupt at NVIC level for  PORT1
void configureUART(void);//Configure UART
#endif