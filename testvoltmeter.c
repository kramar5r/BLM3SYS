/*
#########################################################
#                                                       #
#   BLM3SYS  -> testvoltmeter  	           		        #
#      The programme that is compiled and tested 		#
#      as a voltmeter lowlevel driver test              #
#														#
#   Author: Peter Kramar       Date: 18. 4. 2017        #
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

	float Vin;
	if( argc > 1 ) {
    Vin = atof(argv[1]);  // alternative strtod
    }
	
	if (rp_Init() != RP_OK) {
        fprintf(stderr, "Red Pitaya API init failed!\n");
        return EXIT_FAILURE;
    }

    // All pin startup definition
    OutputPinDefinition(); 
	UpdatePinStructures();
    WriteAllBus();

    setLeds(4);

 
    /*   Testing voltmeter setup   */
    float outV;
    outV = measureVoltage(Vin);
    outV ++;
    //printf("\n Amplification rate from defined voltmeterMatrix %f \n", outV);

    ContiniousAcquire(); // Za zajem signala

	rp_Release();

    return EXIT_SUCCESS;
}
