/*
#########################################################
#                                                       #
#   BLM3SYS  -> testgenerator				                    #
#      The programme for testing signal generator       #
#      functions and amplifier settings                 #
#														                            #
#   Author: Peter Kramar       Date: 23. 6. 2017        #
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

    // DO MORE ON THIS FILE!!!!!
	if( argc > 1 ) {
     x = atof(argv[1]);  // alternative strtod
    }

  else{
    printf("\nUsage: \n");
    printf("testgenerator clamp-type signal-type amplitude frequency mode \n");
    printf("\n");
    printf("clamp-type: [VC|CC|IC] VC - voltage clamp, CC - current clamp, IC - ion channel \n");
    printf("signal-type: [SIN|RAMP|PULSE] \n");
    printf("amplitude: [value] V \n");
    printf("frequency: [value] Hz \n");
    printf("mode: [SING|CONT] Hz \n");
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

  turnOnLed(4);  // setup amplifier




    // Set type of
    setVoltageClamp();


    /*   Testing voltmeter setup   */
 ////   float outV;
 ////   outV = measureVoltage(1.0);
 ////   printf("\n Maximum ratio from the matrix is %f \n", outV);
/* Test OJAČANJA 
    setLeds(2);
    sleep(1);
    setVCResistorsValue(1.0, 3.05);
    setLeds(29);
    singlePulseGeneration();

*/
    singleRampGeneration(48.1, 1.0);   // test rampe  k, Uout

	// ---------------------------------- THIS IS TO START GENERATING CONTINIOUS SIGNAL ----------
    //ContiniusGeneration(1000.0, 1.0);

    /*

    


    // nanosecondpulse triggering
    
    //triggerNSpulse();


    // TOLE JE TESTNA FUNKCIJA KI JO JE PRIPRAVIL ŽUMRE...

    
    


   
    // TOLE JE TREBA ŠE TESTIRAT -----------setVCResistorsValue(1.0, x);
 ////   x++; // TOLE JE SAMO ZATO DA JE X UPORABLJEN



    //printf("CSS selection I3=%d I2=%d I1=%d \n", I1, I2, I3);
   //singlePulseGeneration();

   // printf("End of the programme\n");
    //sleep(5);
   // printf("ACQUISITION TEST:\n");
    //setLeds(0);


    // TESTING ACQUIRE SIGNAL WITH PRINTFDATA
   
   //ContiniousAcquire(); ZA ZAJEM SIGNALA
   
   // printf("CIAO\n");*/

	rp_Release();

    return EXIT_SUCCESS;
}