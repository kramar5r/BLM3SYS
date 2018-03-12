/*
#########################################################
#                                                       #
#   BLM3SYS  -> measureRamp    				                  #
#      The programme for testing signal generating      #
#      or ramp and measuring voltage and current        #
#														                            #
#   Author: Peter Kramar       Date: 17. 8. 2017        #
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


int main(int argc,char *argv[]) {

float amp;
float slope;
int ampmetStage;
int amplification;

int outError;


  if( argc > 1 ) {
    amp = atof(argv[1]);
    slope = atof(argv[2]);  // alternative strtod 
    ampmetStage = atof(argv[3]);
    amplification = atof(argv[4]);
    }

  else{
    printf("\nUsage: \n");
    printf("testConGenerator <Amplitude> <Slope> \n");
    printf("\n");
    printf("Amplitude: [value] V \n");
    printf("Slope: [value] kV/s \n");
    printf("Amperemeter stage: 0 - 10k, 1 - 100k, 2 - 1M, 3 - 10M, 4 - 100M, 5 - 1G, 6 - 10G, 7 - 100G \n");
    printf("Voltmeter-Amplification: 0 = 1x, 1 = 2x, 2 = 5x, 3 = 10x \n");
    printf("\n");
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

    // Amperemeter setup
  outError=setAmpermeter(ampmetStage, 0); // Must be 1. with 0 goes quick to saturation !!!!
  if (outError!=0){
    printf("\n Ampermeter ERROR nr:%d \n", outError);
    return EXIT_FAILURE;
  }

  // Voltmeter setup
  outError=setVoltmeter(amplification, 0);
  if (outError!=0){
    printf("\n Voltmeter ERROR nr:%d \n", outError);
    return EXIT_FAILURE;
  }
  

  // Start the pulse generation
  singleRampGeneration(slope, amp);
  

  // Red Pitaya release
	rp_Release();

  return EXIT_SUCCESS;
}