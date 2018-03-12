/*
#########################################################
#                                                       #
#   BLM3SYS  -> testConGenerator				                #
#      The programme for testing continious             #
#      signal generator functions                       #
#														                            #
#   Author: Peter Kramar       Date:  3. 8. 2017        #
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

// additional function declaration
extern int AM_E;


int main(int argc,char *argv[]) {

float amp;
float freq;
int ampmetStage;
int amplification;
//int division;

//ampermeter parameters
int outError;

  if( argc > 1 ) {
    amp = atof(argv[1]);
    freq = atof(argv[2]);  // alternative strtod 
    ampmetStage = atof(argv[3]);
    amplification = atof(argv[4]);
    // division = atof(argv[5]);
    }

  else{
    printf("\nUsage: \n");
    //printf("testConGenerator clamp-type signal-type Output-Amplitude Frequency \n");
    printf("testConGenerator <Amplitude> <Frequency> <Amperemeter-Stage> <Voltmeter-Amplification> <Vm-division>\n");
    printf("\n");
    //printf("clamp-type: [VC|CC|IC] VC - voltage clamp, CC - current clamp, IC - ion channel \n");
    //printf("signal-type: [SIN|RAMP|PULSE] \n");
    printf("amplitude: [value] V \n");
    printf("frequency: [value] Hz \n");
    printf("Amperemeter stage: 0 - 10k, 1 - 100k, 2 - 1M, 3 - 10M, 4 - 100M, 5 - 1G, 6 - 10G, 7 - 100G \n");
    printf("Voltmeter-Amplification: 0 = 1x, 1 = 2x, 2 = 5x, 3 = 10x \n");
    //printf("Vm-division: 0 = 1x, 1 = 5x \n");
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

  // Set type of generation
  setVoltageClamp();

  // Start the continious signal generation
  ContiniusGeneration(freq, amp);

  
  // start current measurement
  /*
    Switches of current measurement setup are now set here manualy in this file.
    The automatic selection of the level should be programmed later
  */

  outError=setAmpermeter(ampmetStage, 0); // Must be 1. with 0 goes quick to saturation !!!!

  if (outError!=0){
    printf("\n Ampermeter ERROR nr:%d \n", outError);
  }
  else{
    printf("\n Ampermeter Started ... \n");
  }

  outError=setVoltmeter(amplification, 0); // devision has been removed from here

  if (outError!=0){
    printf("\n Voltmeter ERROR nr:%d \n", outError);
  }
  else{
    printf("\n Voltmeter Started ... \n");
  }


  ledDance(1);



  // Stop the continious signal generator
  //disableContGeneration();


  // Red Pitaya release
	rp_Release();

  return EXIT_SUCCESS;
}


