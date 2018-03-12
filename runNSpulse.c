/*
#########################################################
#                                                       #
#   BLM3SYS  -> RUNCODE							        #
#      The programme that is compiled and tested 		#
#      as a funcional test                              #
#														#
#   Author: Peter Kramar       Date: 3. 1. 2017	        #
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


int main() {

	if (rp_Init() != RP_OK) {
        fprintf(stderr, "Red Pitaya API init failed!\n");
        return EXIT_FAILURE;
    }

    // All pin startup definition
    OutputPinDefinition(); 
	UpdatePinStructures();
    WriteAllBus();

    setLeds(6);
    triggerNSpulse();
    setLeds(0);

	rp_Release();

    return EXIT_SUCCESS;
}