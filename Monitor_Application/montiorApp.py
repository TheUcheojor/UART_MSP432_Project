'''
    File: monitorApp.py
    Purpose: The purpose of this file is to communicate with a MSP432 board using UART.
             The application communicates with the MSP432 board via the serial port over 
             a USB connection
    Author: Paul Okenne
'''

import serial
import keyboard

SERIAL_LINE="COM4"
SPEED=9600

ser = serial.Serial(SERIAL_LINE,SPEED)  # open serial port

'''
    Function incrementState
    Purpose: Increment the state of the MSP432 Board by writing byte 'w'
    Parameter: event: event
    Context: This function will be used as a callback function that the package
             keyboard calls when the key 'w' is pressed.
             The package keyboard calls the callback with an event. 
'''
def incrementState(event):
    ser.write(b'w') 

'''
    Function decrementState
    Purpose: Decrement the state of the MSP432 Board by writing byte 's'
    Parameter: event: event
'''
def decrementState(event):
    ser.write(b's')

keyboard.on_release_key('w',incrementState)#call incrementState() when w is pressed
keyboard.on_release_key('s',decrementState)#call decrementState() when s is pressed

#Display a welcome message and instructions
print("Welcome To My UART Project!\nPress w to increment the state\nPress s to decrement the state")
print("-------------------------------------------")

while True:
    print("Current State: "+str(ser.read()))#Read the current state


ser.close()#Closes the port