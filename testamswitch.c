/*
#########################################################
#                                                       #
#   BLM3SYS  -> test amperemeter switch   		        #
#      The programme that is compiled and tested 		#
#      as a amperemetre lowlevel driver test            #
#														#
#   Author: Peter Kramar       Date:  7. 5. 2017        #
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

	int amplification;
	int division;
	int outError;

	if( argc > 1 ) {
    amplification = atof(argv[1]);  // amplification of diferential voltmeter
    division = atof(argv[2]);  // division of probe
    }
    else{
    	printf("\nError using function testamswitch. \n");
    	printf("Usage: testamswitch [amplification] [divison] \n");
    	printf("[amplification] 0 = 500 uA, 1 = 50 uA, 2 = 5 uA, 3 = 500 nA, 4 = 50 nA, 5 = N/A, 6 = N/A, 7 = N/A \n");  
    	printf("[divison] 0 = 1x, 1 = 5x \n"); 
    }
	
	if (rp_Init() != RP_OK) {
        fprintf(stderr, "Red Pitaya API init failed!\n");
        return EXIT_FAILURE;
    }

    // All pin startup definition
    OutputPinDefinition(); 
	UpdatePinStructures();
    WriteAllBus();

    setLeds(6);

 
    /*   Testing Ampermeter switching   */
    outError=setAmpermeter(amplification, division);
    
    printf("\n Output message is %d \n", outError);

    

	rp_Release();

    return EXIT_SUCCESS;
}
