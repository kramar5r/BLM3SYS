/*
#########################################################
#                                                       #
#   BLM3SYS  -> RUNCODE							                    #
#      The programme that is compiled and tested 		    #
#      as a funcional test                              #
#														                            #
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



int main(int argc,char *argv[]) {

  float Vin;
  float Vout;
  
	if( argc > 1 ) {
     Vin = atof(argv[1]);  // alternative strtod
     Vout = atof(argv[2]);
     printf("\nInserted parameterts OK: Vin = %f and Vout = %f \n", Vin, Vout);
    }
	
  else{
    printf("\nERROR USAGE FUNCTION\n");
    printf("runcode vin vout\n");
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

    turnOnLed(1); // Starting sistem
    

    
    // Set type of clamp
    setVoltageClamp();
    setVCResistorsValue(Vin, Vout);

    turnOnLed(4); // Amplification Setup


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

    turnOnLed(5);

    ContiniusGeneration(1000.0, 1.0);


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