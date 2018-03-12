/*
#########################################################
#                                                       #
#   BLM3SYS  -> Leds (5) operation and command 	        #
#           	FUNCTION FILE							#
#                                                       #
#   Author: Peter Kramar       Date: 3. 1. 2017	        #
#########################################################

List of Functions:

setLeds(int ledCodeNum)
turnOnLed(int ledNum)
turnOffLed(int ledNum)
ledDance(int cycles)

*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "redpitaya/rp.h"

#include "BLM3SYS-leds.h"
#include "BLM3SYS-latches.h"



int setLeds(int ledCodeNum){
	// here comes the code where you set the leds as a number and write it to the bits and to the leds latches.
	int bin_vect[5];

	// Initial checking if the number is smaller than 31 if not return a Failure exit
	if (ledCodeNum > 31){
        printf("ERROR: Number is grater than 31. \n");
        return EXIT_FAILURE;
    }

    
    // Decimal to Binnary conversion 

    // bin_vect [bit4, bit3, bit2, bit1, bit0, NON*]  
    //    i       0      1     2     3     4    5
    // NON* element is an empty element that will not be used in the project. But its apearance helps 
    // with the while sentances
    
    int i=0;
    int k;
    int rem;
    int n = ledCodeNum;
    
    while(n != 0){
        rem = n%2;
        n = n/2;
        //printf("i= %d in n= %d  ostanek: %d ...\n",i,n,rem);
        bin_vect[i]=rem;
        //printf("%d - That is i \n",i);
        i++;
    }
    
    // if the binary vector is not full this while will place the zeros on uper bits if necesery...
    for (k=i;k<5;k++){
        bin_vect[k]=0;            
    }
	

    // Writting converted values to the led pin variables
    LED0=bin_vect[4];
    LED1=bin_vect[3];
    LED2=bin_vect[2];
    LED3=bin_vect[1];
    LED4=bin_vect[0];


	// Write pin values to the LED latches on subbus N6
	UpdatePinStructures();
    writeOnN6();

	return EXIT_SUCCESS;
}


int turnOnLed(int ledNum){
	
	// Function to turn on LED diode. The argument ledNum stands for number of led on house
	/*
			[X](blue)	[X](red)	[X](yellow)		[X](green)		[X](blue)
			ledNum = 0	ledNum = 1	ledNum = 2		ledNum = 3		ledNum = 4
			Led0		Led1		Led2			Led3			Led4
	*/

	switch(ledNum){
		case 0 : LED0=1; break;
		case 1 : LED1=1; break;
		case 2 : LED2=1; break;
		case 3 : LED3=1; break;
		case 4 : LED4=1; break;
		default : return EXIT_FAILURE;
	}
	
    UpdatePinStructures();
    writeOnN6();

	return EXIT_SUCCESS;
}


int turnOffLed(int ledNum){
	
	// Function to turn OFF LED diode. The argument ledNum stands for number of led on house
	/*
			[X](blue)	[X](red)	[X](yellow)		[X](green)		[X](blue)
			ledNum = 0	ledNum = 1	ledNum = 2		ledNum = 3		ledNum = 4
			Led0		Led1		Led2			Led3			Led4
	*/

	switch(ledNum){
		case 0 : LED0=0; break;
		case 1 : LED1=0; break;
		case 2 : LED2=0; break;
		case 3 : LED3=0; break;
		case 4 : LED4=0; break;
		default : return EXIT_FAILURE;
	}
	
    UpdatePinStructures();
    writeOnN6();
	
	return EXIT_SUCCESS;

}

void ledDance(int cycles){
for (int j=0;j<=cycles;j++){
    for (int i=0;i<32;i+=1){  //Samo povečuje za 2 da je prej konc
      setLeds(i);
      usleep(100000);
    }
  } 
}