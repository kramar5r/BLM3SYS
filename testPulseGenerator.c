/*
#########################################################
#                                                       #
#   BLM3SYS  -> testPulsGenerator				                #
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


int main(int argc,char *argv[]) {

float amp;
float freq;
int Nburst;

  if( argc > 1 ) {
    amp = atof(argv[1]);
    freq = atof(argv[2]);  // alternative strtod 
    Nburst = atof(argv[3]);
    }

  else{
    printf("\nUsage: \n");
    //printf("testConGenerator clamp-type signal-type Output-Amplitude Frequency \n");
    printf("testConGenerator <Amplitude> <Frequency> <Number of Bursts>\n");
    printf("\n");
    //printf("clamp-type: [VC|CC|IC] VC - voltage clamp, CC - current clamp, IC - ion channel \n");
    //printf("signal-type: [SIN|RAMP|PULSE] \n");
    printf("Amplitude: [value] V \n");
    printf("Frequency: [value] Hz \n");
    printf("Number of Bursts: [value] \n");
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
  singlePulseGeneration(amp, freq, Nburst);
  

  // Red Pitaya release
	rp_Release();

  return EXIT_SUCCESS;
}