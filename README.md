# UART_MSP432_Project

The monitoring application was developed using Python. Python provides a pyserial library, which when paired with its keyboard library, provides tools to write and read the current state of the embedded board. 

Install the pyserial library:
```pip install pyserial```

Install the keyboard library:
```pip install keyboard``


The application runs a while loop that prints the current state of the board when it is updated on the application or the embedded board. The monitoring application also has keyboard interrupts for ‘w’ and ‘s’. The ‘w’ keyboard interrupt increments the state of the embedded board by one; the ‘s’ keyboard interrupt decrements the state of the embedded board by one. State increments work by sending the byte ‘w’ to the board; state decrements work by sending the byte ‘s’ to the board.

The embedded board application allows for LEDs state changes through button clicks and serial data sent to the board’s receiver. It outputs the current state to the board’s transmitter which transfers the serial data to the computer’s receiver.

Figure 1 displays my code architecture. The project_header file defines configuration functions that are implemented by project_support. 

!()[Images/CodeArchitecture]
Figure 1: Code Architecture

The term_project file initiates the configuration functions and implements a state machine as shown in Figure 2.

!()[Images/StateDiagram]
Figure 2: State Diagram of Embedded Board Application		

There are four states: in order, they are LED 1 OFF/LED 2 OFF, LED 1 ON/LED 2 OFF, LED 1 OFF/LED 2 ON, and LED 1 ON /LED 2 ON. 

		
 
To control the state, one can use  the  board buttons. The PORT1 ISR checks if Button 1(port 1 - pin 1) is pressed, increasing the state by one; if Button 2(port 1 - pin 4) is pressed, the state decreases by one.

One can also change the state through serial communication. The UART ISR checks if ‘w’ or ‘s’ has been sent to the board's receiver. If ‘w’ has been received, the state increases by one; if ‘s’ has been received, the state decreases by one.

In term_project.c, the function changeState, which takes in state increment amount, changes the state of the system. It adds the state increment amount, setting the LEDs to the new state. The changeState function aso outputs the new state with the board’s transmitter.
