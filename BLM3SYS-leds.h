/*
#########################################################
#                                                       #
#   BLM3SYS  -> Leds variable deffinition	 	        #
#           	HEADER FILE								#
#                                                       #
#   Author: Peter Kramar       Date: 12. 1. 2017	    #
#########################################################
*/

#ifndef leds_H
#define leds_H

// Global variable definition
extern int LED0;		// < DIO0_P
extern int LED1;		// < DIO1_P
extern int LED2;		// < DIO2_P
extern int LED3;		// < DIO3_P
extern int LED4;		// < DIO4_P


// Function definition
int setLeds(int ledCodeNum);
int turnOnLed(int ledNum);
int turnOffLed(int ledNum);
void ledDance(int cycles);


#endif