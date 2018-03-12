/*
#########################################################
#                                                       #
#   BLM3SYS  -> testRampGenerator				                #
#      The programme for testing continious             #
#      signal generator functions                       #
#														                            #
#   Author: Peter Kramar       Date:  4. 8. 2017        #
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


  if( argc > 1 ) {
    amp = atof(argv[1]);
    slope = atof(argv[2]);  // alternative strtod 
    }

  else{
    printf("\nUsage: \n");
    printf("testConGenerator <Amplitude> <Slope> \n");
    printf("\n");
    printf("Amplitude: [value] V \n");
    printf("Slope: [value] kV/s \n");
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

  // Start the pulse generation
  singleRampGeneration(slope, amp);
  

  // Red Pitaya release
	rp_Release();

  return EXIT_SUCCESS;
}