/*
#########################################################
#                                                       #
#   BLM3SYS  -> testampermeter 	           		        #
#      The programme that is compiled and tested 		#
#      as a ampermeter lowlevel driver test             #
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

	float Iin;
	if( argc > 1 ) {
    Iin = atof(argv[1]);  // alternative strtod
    }
    else{
        printf("Usage: testampermeter Iin \n");
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

    setLeds(7);

 
    /*   Testing ampermeter setup   */
    float outI;
    outI = measureCurrent(Iin);
    printf("\n Amplification rate from defined voltmeterMatrix %f \n", outI);

    // TUKAJ BO POTREBEN ŠE KAKŠEN IZRAČUN MOGOČE? NEKAKO BO POTREBNO DOBITI TOK IZ IZMERJENE NAPETOSTI! FAKTOR BO LE V UPORNOSTI, 
    // KI JO BOMO LE MNOŽILI 7.5.2017



	rp_Release();

    return EXIT_SUCCESS;
}
