/*
#########################################################
#                                                       #
#   BLM3SYS  -> capture.c				                #
#      The programme for capturing data of              #
#      voltmeter and amperemeter and store		        #
#	   them to measure.txt		                        #
#														#
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

  int decimation;
  int error;
  
  
	if( argc > 1 ) {
     decimation = atof(argv[1]);  // alternative strtod
    }
	
  else{

  	printf("\nUsage: \n");  /// TOLE JE TREBA ZA CAPTURE VSE AMPAK VSE SPREMENIT
    //printf("testConGenerator clamp-type signal-type Output-Amplitude Frequency \n");
    printf("testConGenerator <Amplitude> <Frequency> <Amperemeter-Stage> \n");
    printf("\n");
    //printf("clamp-type: [VC|CC|IC] VC - voltage clamp, CC - current clamp, IC - ion channel \n");
    //printf("signal-type: [SIN|RAMP|PULSE] \n");
    printf("amplitude: [value] V \n");
    printf("frequency: [value] Hz \n");
    printf("Amperemeter stage: 0 - 10k, 1 - 100k, 2 - 1M, 3 - 10M, 4 - 100M, 5 - 1G, 6 - 10G, 7 - 100G \n");
    

    // POPRAVI TO SPODAJ TAKO KOT BI MOGL BIT ZGOR!!!!

    printf("\nERROR USAGE FUNCTION\n");
    printf("capture <decimation>\n");
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

   
  	error=ContiniousAcquire(decimation); 

	rp_Release();

  return error;
}