/*
#########################################################
#                                                       #
#   BLM3SYS  -> testleds      	           		        #
#      The programme that is compiled and tested 		#
#      as a LED diodes lowlevel driver test             #
#														#
#   Author: Peter Kramar       Date:  8. 5. 2017        #
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

	int data;
    int mode;
	int error = 0;

    if( argc > 1 ) {
    data = atof(argv[1]);  // numerical data for led diodes
    mode = atof(argv[2]); // mode for data 
    }
    else{
        printf("\nError using function testleds. \n");
        printf("Usage: testleds [data] [mode] \n");
        printf("[data] Decimal number depends on mode \n");  
        printf("[mode] 0 = leds off, 1 = leds on, 2 setleds, 3 binnary count \n"); 
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

    
    switch(mode){
        case 0 :
            printf("mode 0 - turnOffLed()");
            turnOffLed(data);
            break;

        case 1 :
            printf("mode 1 - turnOnLed");
            turnOnLed(data);
            break;

        case 2 :
            printf("mode 2 - setleds()");
            setLeds(data);
            break;

        case 3 :
            printf("mode 3 - sweep leds");
            for (int j=0;j<=data;j++){
                for (int i=0;i<32;i+=1){  //Samo povečuje za 2 da je prej konc
                    setLeds(i);
                    usleep(100000);
                }
            } 
            break;

        default : 
            error += 1;
    }

    

 

	rp_Release();

    return error;
}
