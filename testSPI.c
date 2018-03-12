/*
#########################################################
#                                                       #
#   BLM3SYS  -> testSPI		 	           		        #
#      The programme that is compiled and tested 		#
#      as a SPI communication protocol on RedPitaya     #
#														#
#   Author: Peter Kramar       Date: 25. 5. 2017        #
#########################################################
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "redpitaya/rp.h"
#include "BLM3SYS-globals.h"

#include "BLM3SYS-latches.h"
#include "BLM3SYS-leds.h"
#include "BLM3SYS-generator.h"
#include "BLM3SYS-spi.h"


int main(int argc,char *argv[]){

	float VINT;
	float VOUT;

	if( argc > 2 ) {
     VINT = atof(argv[1]);  // input voltage
     VOUT = atof(argv[2]);  // voltage output
    }
    else{
    	printf("\n Something goes wrong... \n");
    	printf("\n Usage: testSPI Vin Vout \n");
    	printf("\n Where Vin and Vout are float values \n");
    	return EXIT_FAILURE;
    }

	if (rp_Init() != RP_OK) {
        fprintf(stderr, "Red Pitaya API init failed!\n");
        return EXIT_FAILURE;
    }

	
    turnOnLed(0);
    sleep(1);

	// All pin startup definition
    OutputPinDefinition(); 
	UpdatePinStructures();
    WriteAllBus();

    setVoltageClamp();

    int *tab;
	int i =0;
	tab = malloc(4*sizeof(int));

	turnOnLed(1);
	sleep(1);

    printf("\n Test of function FindResistors: \n");
    tab = FindResistors(VINT,VOUT);
	
	for(i=0;i<4;i++) printf("%d\n",tab[i]);    // Tale vrstica naredi izpis za izračunane komponente
        
	turnOffLed(1);
	turnOnLed(2);
	sleep(1);
    setVCResistorsValue(VINT, VOUT);
	turnOffLed(2);

	sleep(1);
/*	printf("\n TESTING SPI-WRITE COMMAND  \n");
	turnOnLed(3);
	sleep(1);

	//char *data[3]={0x254, 0x33, 0};
	char *data="AaBbCc";
	//char *data[8] ={0x01, 0x05, 0x00, 0x20, 0xFF, 0x00, 0x00, 0x00};

	if (write_spi(data, strlen(data))<0){
		printf("Write to SPI FAILED");
	}

	turnOffLed(3);
	sleep(1);*/
	turnOffLed(0);

    rp_Release();

	return EXIT_SUCCESS;
}