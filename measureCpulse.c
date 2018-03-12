/*
#########################################################
#                                                       #
#   BLM3SYS  -> measureCpulse   				                #
#      The programme for measuring capacitance          #
#      by method of discharging                         #
#														                            #
#   Author: Peter Kramar       Date: 14. 8. 2017        #
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
  int ampmetStage;    // input amperemeter argument
  int decimation;     // input parameter of decimation

  int outError=0;     // programme error handle


  if( argc > 1 ) {
    amp = atof(argv[1]);
    freq = atof(argv[2]);  
    ampmetStage = atof(argv[3]);
    decimation = atof(argv[4]);
    }

  else{
    printf("\nUsage: \n");
    printf("testConGenerator <Amplitude> <Frequency> <Amperemeter-Stage> <Decimation>\n");
    printf("\n");
    printf("Amplitude: [value] V \n");
    printf("Frequency: [value] Hz \n");
    printf("Amperemeter stage: 0 - 10k, 1 - 100k, 2 - 1M, 3 - 10M, 4 - 100M, 5 - 1G, 6 - 10G, 7 - 100G \n");
    printf("Decimation: 1, 8, 64, 1024, 8192, 65536 and 0 for skip\n");
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
  dischargePulseGenerator(amp, freq);

  // setup amperemeter
  outError+=setAmpermeter(ampmetStage, 0); // Must be 1. with 0 goes quick to saturation !!!!
  if (outError!=0){
    printf("\n Ampermeter ERROR nr:%d \n", outError);
    ledDance(2);
    }
  else{
    turnOnLed(2);
  }

  // Setup voltmeter
  float outV;
  outV = measureVoltage(amp);
  turnOnLed(3);
  outV++;

  // // PART COPIED FROM THE GENERATOR CODE NEEDS TO BE SYNCED HERE WITH ACQUISITON
  // rp_GenTrigger(RP_CH_1);
  //   usleep(500);
  //   rp_GenOutEnable(RP_CH_1);
  //   usleep(30);  // This needs to be changed due to pulse durattion
    
  //   // Here we need to add code to turn of the switch to measure capacitance!!!!!!!!!!


  //   // Turn off the signal generator! 
  //   rp_GenOutDisable(RP_CH_1);

  // // -------------------END OF CODE --------------------------

    // HERE WE NEED TO SETUP EVERY THING FOR ACQUSITION AND THEN START A 
    // function for single acqusition


  // Measurement of applied signal!!!!
  outError+=SingleAcquire(decimation); 
  turnOnLed(4);

  // ...... END OF THE MEASUREMENT PART TO BE ADDED
  

  // Red Pitaya release
	rp_Release();

  return EXIT_SUCCESS;
}