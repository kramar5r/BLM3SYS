/*
#########################################################
#                                                       #
#   BLM3SYS  -> measureConSin.c				            #
#      The programme apply continious sinus signal,     #
#      measures voltage and current.					#
#	   Use _______.py to run this code.                 #
#							                            #
#   Author: Peter Kramar       Date:  9. 8. 2017        #
#########################################################
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "redpitaya/rp.h"
#include "BLM3SYS-globals.h"
#include "BLM3SYS-latches.h"
#include "BLM3SYS-leds.h"
#include "BLM3SYS-generator.h"
#include "BLM3SYS-spi.h"
#include "BLM3SYS-acquire.h"
#include "BLM3SYS-uart.h"

// additional function declaration
extern int AM_E;


int main(int argc,char *argv[]) {
	
	float amp;				// input amplitude argument
	float freq;				// input frequency argument
	int ampmetStage;		// input amperemeter argument
	// VOLTMETER SETUP not needed
	int decimation;			// input parameter of decimation
	//int samplingRate;

	int outError=0;			// programme error handle


	// Evaluating the parameters
	if( argc > 1 ) {
		amp = atof(argv[1]);
		freq = atof(argv[2]);  
		ampmetStage = atof(argv[3]);
		// voltmeter stage not needed
		decimation = atof(argv[4]);
		//samplingRate = atof(argv[5]);
    }
    else{
    	printf("\nUsage: \n");
    	printf("testConGenerator <Amplitude> <Frequency> <Amperemeter-Stage> <decimation> <Sampling-Rate>\n"); // HERE TO ADD VOLTMETER
    	printf("\n");
    	printf("amplitude: [value] V \n");
    	printf("frequency: [value] Hz \n");
    	printf("Amperemeter stage: 0 - 10k, 1 - 100k, 2 - 1M, 3 - 10M, 4 - 100M, 5 - 1G, 6 - 10G, 7 - 100G \n");
    	// Voltmeter uses the amp parameter!!!
    	printf("Decimation: 1, 8, 64, 1024, 8192, 65536 and 0 for skip\n");
    	//printf("Sampling Rate: 1, 8, 64, 1024, 8192, 65536 and 0 for skip\n");
    	return EXIT_FAILURE;
  	}

	if (rp_Init() != RP_OK) {
        fprintf(stderr, "Red Pitaya API init failed!\n");
        return EXIT_FAILURE;
    }

 	// All pin startup definition
	OutputPinDefinition(); 
	UpdatePinStructures();
	WriteAllBus();

	/*
	# A led dance cycle to have setup ?This might took to much time
	*/
	//ledDance(1);
	setLeds(0);

	// Set type of generation
  	setVoltageClamp();
  	turnOnLed(0);

  	// Start the continious signal generation
  	ContiniusGeneration(freq, amp);
  	turnOnLed(1);

  	// setup amperemeter
  	outError+=setAmpermeter(ampmetStage, 0); // Must be 1. with 0 goes quick to saturation !!!!
  	if (outError!=0){
  		printf("\n Ampermeter ERROR nr:%d \n", outError);
  		ledDance(2);
  	}
  	else{
  		turnOnLed(2);
  	}

 	// Setup voltmeter
 	float outV;
    outV = measureVoltage(amp);
    turnOnLed(3);
    outV++;


 	// Measurement of applied signal!!!!
 	outError+=ContiniousAcquire(decimation); 
 	turnOnLed(4);

 	// Stop the continious signal generator
 	disableContGeneration();
 	setLeds(0);

	// Red Pitaya release
	rp_Release();

	return EXIT_SUCCESS;
}


