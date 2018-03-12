/*
#########################################################
#                                                       #
#   BLM3SYS  -> Acquire signal and measurement			#
#           	FUNCTION FILE							#
#                                                       #
#   Author: Peter Kramar       Date:  9. 3. 2017        #
#########################################################

List of Functions:

int ContiniousAcquire()
int setAmpermeter(int ampermeterAmp, int probeDivAm)
int setVoltmeter(int voltmeterAmp, int probeDiv)
int measureVoltage(float setUinPP)  Output is Voltage range that is calculated...
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "redpitaya/rp.h"


#include "BLM3SYS-leds.h"
#include "BLM3SYS-latches.h"
#include "BLM3SYS-acquire.h"

// to be fixed and defined in header
const float voltmeterMatrix[2][4]=
{
    {1.0, 2.0, 5.0, 10.0},
    {0.20, 0.40, 1.0, 2.0}
};

const long double ampermeterMatrix[2][8]=
{
	{10000.0, 100000.0, 1000000.0, 10000000.0, 100000000.0, 1000000000.0, 10000000000.0, 100000000000.0},
	{2000.0, 20000.0, 200000.0, 2000000.0, 20000000.0, 200000000.0, 2000000000.0, 20000000000.0}
};


int ContiniousAcquire(int decimation){

	int error = EXIT_SUCCESS;
	// TOLE MORA BIT VERJETNO DEFINIRAN ŽE EN NIVO VIŠJE
	// OSNOVNA DEFINICIJA uint32_t buff_size = 16384;
	uint32_t buff_size = 1638;  // 10x manj
	
	float *buff_ch1 = (float *)malloc(buff_size * sizeof(float));
	float *buff_ch2 = (float *)malloc(buff_size * sizeof(float));

	/*
	This function enables pulling acquisition for continious measurement. The result of measurement can be 
	calculated using a developed algorythms. Called function returns values. After that mesurement can be 
	written into the file or processed...
	*/

	rp_AcqReset();

	/*
		Setup the Sample rate with the number of decimation:
		RP_DEC_1 		Sample rate 125Msps; Buffer time length 131us; Decimation 1.
		RP_DEC_8 		Sample rate 15.625Msps; Buffer time length 1.048ms; Decimation 8.
		RP_DEC_64 		Sample rate 1.953Msps; Buffer time length 8.388ms; Decimation 64.
		RP_DEC_1024 	Sample rate 122.070ksps; Buffer time length 134.2ms; Decimation 1024.
		RP_DEC_8192 	Sample rate 15.258ksps; Buffer time length 1.073s; Decimation 8192.
		RP_DEC_65536 	Sample rate 1.907ksps; Buffer time length 8.589s; Decimation 65536.

	*/

	switch(decimation){
		case 0 :
			break;

		case 1 : 
			error += rp_AcqSetDecimation(RP_DEC_1);
			break; // Sample rate 125Msps; Buffer time length 131us; Decimation 1.

		case 8 :
			error += rp_AcqSetDecimation(RP_DEC_8);
			break; // Sample rate 15.625Msps; Buffer time length 1.048ms; Decimation 8.

		case 64 :
			error += rp_AcqSetDecimation(RP_DEC_64);
			break; // Sample rate 1.953Msps; Buffer time length 8.388ms; Decimation 64.

		case 1024 : 
			error += rp_AcqSetDecimation(RP_DEC_1024);
			break; // Sample rate 122.070ksps; Buffer time length 134.2ms; Decimation 1024.

		case 8192 :
			error += rp_AcqSetDecimation(RP_DEC_8192);
			break; // Sample rate 15.258ksps; Buffer time length 1.073s; Decimation 8192.

		case 65536 :
			error += rp_AcqSetDecimation(RP_DEC_65536);
			break; // Sample rate 1.907ksps; Buffer time length 8.589s; Decimation 65536.

		default :
			printf("\n Invalid decimation value used. \n");
			printf("Please use 1, 8, 64, 1024, 8192, 65536 and 0 for break \n");
			error = EXIT_FAILURE;
	}

	/*
	RP_SMP_125M,     //!< Sample rate 125Msps; Buffer time length 131us; Decimation 1
    RP_SMP_15_625M,  //!< Sample rate 15.625Msps; Buffer time length 1.048ms; Decimation 8
    RP_SMP_1_953M,   //!< Sample rate 1.953Msps; Buffer time length 8.388ms; Decimation 64
    RP_SMP_122_070K, //!< Sample rate 122.070ksps; Buffer time length 134.2ms; Decimation 1024
    RP_SMP_15_258K,  //!< Sample rate 15.258ksps; Buffer time length 1.073s; Decimation 8192
    RP_SMP_1_907K    //!< Sample rate 1.907ksps; Buffer time length 8.589s; Decimation 65536
	*/
    // THIS PART HAS BEEN REMOVED DUE TO THE FACT THAT ONLY ONE FUNCTION WILL TAKE PLACE
    /*
    switch(samplingRate){
    	case 0 :
    		break;

    	case 1 :
    		error += rp_AcqSetSamplingRate(RP_SMP_125M);
    		break;

    	case 8 :
    		error += rp_AcqSetSamplingRate(RP_SMP_15_625M);
    		break;

    	case 64 :
    		error += rp_AcqSetSamplingRate(RP_SMP_1_953M);
    		break;

    	case 1024 :
    		error += rp_AcqSetSamplingRate(RP_SMP_122_070K);
    		break;

    	case 8192 :
    		error += rp_AcqSetSamplingRate(RP_SMP_15_258K);
    		break;

    	case 65536 :
    		error += rp_AcqSetSamplingRate(RP_SMP_1_907K);
    		break;

    	default :
			printf("\n Invalid samplingRate value used. \n");
			printf("Please use 1, 8, 64, 1024, 8192, 65536 and 0 for break \n");
			error = EXIT_FAILURE;
    }
	*/
	

	if(error==0){
		rp_AcqSetTriggerDelay(0);  // 
		rp_AcqStart();

	/* 
	After acquisition is started some time delay is needed in order to acquire fresh samples in to buffer
	Here we have used time delay of one second but you can calculate exact value taking in to account buffer
	length and smaling rate
	*/

		sleep(1);  // !!!!TEST IF IS REALY NESSESERY TO HAVE ONE SECOND TO WAIT
		rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW);
		rp_acq_trig_state_t state = RP_TRIG_STATE_TRIGGERED;

		while(1){
	                rp_AcqGetTriggerState(&state);
	                if(state == RP_TRIG_STATE_TRIGGERED){
	                sleep(1);
	                break;
	                }
	        }

	    rp_AcqGetLatestDataV(RP_CH_1, &buff_size, buff_ch1);
	    rp_AcqGetLatestDataV(RP_CH_2, &buff_size, buff_ch2);

	    int i;
	    for(i = 0; i < buff_size; i++){
	    	printf("%f\t%f\n", buff_ch1[i], buff_ch2[i]);
	    }
	     	
	}
	   

    // TOLE MORA BIT NA KONCU
	/* Releasing resources */
    
    free(buff_ch1);
	free(buff_ch2);


	// PREGLEJ SITUACIJO!!!
    return(error);

}



int setAmpermeter(int ampermeterAmp, int probeDivAm){

	/*
	Function to setup amplification of the amperemeter by selecting the resistor at 
	Current to voltage converter. Resistors are selected using analog demultiplexor 
	74HC4051. 
	*/

	int error=0;

	/*
	Setting amperemeter amplification number and amperemeter range

	DMUX address |  int ampermeterAmp	|  Used Resistor 	| 	Range 	|
	S2	S1	S0	 | 						| 					|           |
	---------------------------------------------------------------------
	0 	0 	0 	 |			0 			| 		 10 k 		|	500 uA 	|
	0 	0 	1 	 |			1 			|  		100 k 		|	 50 uA 	|
	0 	1 	0 	 |			2 			|		  1 M 		|	  5 uA 	|
	0 	1 	1 	 |			3 			|		 10 M 		|	500 nA 	|
	1 	0 	0 	 |			4 			|     	100 M 		| 	 50 nA 	|
	1 	0 	1 	 |			5 			|		---	 		|	---	 	|
	1 	1 	0 	 |			6 			|		---	 		|	---	 	|
	1 	1 	1 	 |			7 			|		---	 		|	---	 	|
	---------------------------------------------------------------------			
	*/

	switch(ampermeterAmp){
		case 0 :
			AM_S0 = 0;
			AM_S1 = 0;
			AM_S2 = 0;
			break;

		case 1 :
			AM_S0 = 1;
			AM_S1 = 0;
			AM_S2 = 0;
			break;

		case 2 :
			AM_S0 = 0;
			AM_S1 = 1;
			AM_S2 = 0;
			break;

		case 3 :
			AM_S0 = 1;
			AM_S1 = 1;
			AM_S2 = 0;
			break;

		case 4 :
			AM_S0 = 0;
			AM_S1 = 0;
			AM_S2 = 1;
			break;

		case 5 :
			AM_S0 = 1;
			AM_S1 = 0;
			AM_S2 = 1;
			break;
			
		case 6 :
			AM_S0 = 0;
			AM_S1 = 1;
			AM_S2 = 1;
			break;

		case 7 :
			AM_S0 = 1;
			AM_S1 = 1;
			AM_S2 = 1;
			break;

		default : 
			error += 1;
	}

	/*
	Setting up the ampermeter probe division
	int probeDiv		|	Probe division
	---------------------------------------
		0 				| 		1x
		1 				|	    5x
	--------------------------------------
	*/

	switch(probeDivAm){
		case 0 :		// 1x
			AM_RC_SMA=1;
			break;

		case 1 :		// 5x
			AM_RC_SMA=0;
			break;

		default : 
			error += 2;
	}

	//printf("AM_E is 1");
	//AM_E = 1;

	if(error==0){
		UpdatePinStructures();
		writeOnN0();
	}




	return(error);
}

float measureCurrent(float currentRange){

	/*
	Algorithm to calculate the best value of amplification input current
	and then call the setAmpermeter() function
	
	This function returns maximumCurrentRatio value that corresponds for 
	recalculating measured voltage back to current
	*/

	float maximumCurrentRatio=0;

	int iamp;
	int adiv;
	int setadiv;
	int setiamp;
	float workingAMatrix[2][8];

	// scalar multiplication of setted current to ampermeter matrix
	for(iamp=0;iamp<8;iamp++){
		for(adiv=0;adiv<2;adiv++){
			workingAMatrix[adiv][iamp]=ampermeterMatrix[adiv][iamp]*currentRange;
		}
	} 

	// Selecting higest value of ampermeter ratio, where all values higer than 1
	// should be excluded   TRENUTNO SAMO IZPIS MATRIKE!!!!
	for(adiv=0;adiv<2;adiv++){
		for(iamp=0;iamp<4;iamp++){
			printf("%f, ", workingAMatrix[adiv][iamp]);
		}
		printf("... \n");
	}
	// IZPIS MATRIKE ZGORAJ LAHKO ZBRIŠEMO

	for(iamp=0;iamp<4;iamp++){
		for(adiv=0;adiv<2;adiv++){
			if (workingAMatrix[adiv][iamp]>maximumCurrentRatio && workingAMatrix[adiv][iamp]<=1){
				maximumCurrentRatio=workingAMatrix[adiv][iamp];
				setiamp=iamp;
				setadiv=adiv;
			}
		}
	}

	printf("\n amp %d and div %d \n", setiamp, setadiv); // comment this line after!

	// setup the AD8520ARMZ (IC13)
	setAmpermeter(setiamp, setadiv);

	return maximumCurrentRatio;

}




int setVoltmeter(int voltmeterAmp, int probeDiv){   

	/*
	Function to setup IC to setup address pin A0 and A1 and probe division x5 or x1
	voltmeterAmp and probeDiv corresponds to voltmeter amplification setup and
	probe devision type. Function setVoltmeter can be used by algorithm with 
	seting voltmeterMatrix.
	*/

	int error=0;

	/*
	Setting up the voltmeter amplification
	int voltmeterAmp	|	Amplification
	---------------------------------------
		0 				| 		1x
		1 				|		2x
		2 				|		5x
		3 				| 	   10x
	--------------------------------------
	*/

	switch(voltmeterAmp){
		case 0 :	//  1x
			A1=0;
			A0=0;
			break;

		case 1 :	//  2x
			A1=0;
			A0=1;
			break;

		case 2 :	//  5x
			A1=1;
			A0=0;
			break;

		case 3 :	// 10x
			A1=1;
			A0=1;
			break;

		default : 
			error += 1;
	}

	/*
	Setting up the voltmeter probe division
	int probeDiv		|	Probe division
	---------------------------------------
		0 				| 		1x
		1 				|	    5x
	--------------------------------------
	*/

	switch(probeDiv){
		case 0 :		// 1x
			VM_RC_SMA=1;
			break;

		case 1 :		// 5x
			VM_RC_SMA=0;
			break;

		default : 
			error += 2;
	}

	if(error==0){
		UpdatePinStructures();
		writeOnN6();
	}
	

	return error;
}


// algoritem za računanje napetosti vhod naj bo Uin_pp

float measureVoltage(float setUinPP){

	float maximumRatio=0;
	int vamp;
	int pdiv;
	int setpdiv;
	int setvamp;
	float workingVMatrix[2][4];

	// scalar multiplication of setted voltage to voltmeter matrix
	for(vamp=0;vamp<4;vamp++){
		for(pdiv=0;pdiv<2;pdiv++){
			workingVMatrix[pdiv][vamp]=voltmeterMatrix[pdiv][vamp]*setUinPP;
		}
	}

	// Selecting higest value of voltmeter ratio, where all values higer than 1
	// should be excluded   TRENUTNO SAMO IZPIS MATRIKE!!!!
	/*for(pdiv=0;pdiv<2;pdiv++){
		for(vamp=0;vamp<4;vamp++){
			printf("%f, ", workingVMatrix[pdiv][vamp]);
		}
		printf("... \n");
	}*/
	// IZPIS MATRIKE ZGORAJ LAHKO ZBRIŠEMO

	for(vamp=0;vamp<4;vamp++){
		for(pdiv=0;pdiv<2;pdiv++){
			if (workingVMatrix[pdiv][vamp]>maximumRatio && workingVMatrix[pdiv][vamp]<=1){
				maximumRatio=workingVMatrix[pdiv][vamp];
				setvamp=vamp;
				setpdiv=pdiv;
			}
		}
	}

	//printf("\n amp %d and div %d \n", setvamp, setpdiv); // comment this line after!

	// setup the AD8520ARMZ (IC13)
	setVoltmeter(setvamp, setpdiv);

	return voltmeterMatrix[setpdiv][setvamp];  // THIS IS NEW FROM ATHENE TRIP
}


int SingleAcquire(int decimation){

	// For the start we will here setup the 

	int error = EXIT_SUCCESS;
	// TOLE MORA BIT VERJETNO DEFINIRAN ŽE EN NIVO VIŠJE
	// OSNOVNA DEFINICIJA uint32_t buff_size = 16384;
	uint32_t buff_size = 1638;  // 10x manj
	
	float *buff_ch1 = (float *)malloc(buff_size * sizeof(float));
	float *buff_ch2 = (float *)malloc(buff_size * sizeof(float));

	// TESTING

	float *OLD_buff_ch1 = (float *)malloc(buff_size * sizeof(float));
	float *OLD_buff_ch2 = (float *)malloc(buff_size * sizeof(float));

	/*
	This function enables pulling acquisition for continious measurement. The result of measurement can be 
	calculated using a developed algorythms. Called function returns values. After that mesurement can be 
	written into the file or processed...
	*/

	rp_AcqReset();

	/*
		Setup the Sample rate with the number of decimation:
		RP_DEC_1 		Sample rate 125Msps; Buffer time length 131us; Decimation 1.
		RP_DEC_8 		Sample rate 15.625Msps; Buffer time length 1.048ms; Decimation 8.
		RP_DEC_64 		Sample rate 1.953Msps; Buffer time length 8.388ms; Decimation 64.
		RP_DEC_1024 	Sample rate 122.070ksps; Buffer time length 134.2ms; Decimation 1024.
		RP_DEC_8192 	Sample rate 15.258ksps; Buffer time length 1.073s; Decimation 8192.
		RP_DEC_65536 	Sample rate 1.907ksps; Buffer time length 8.589s; Decimation 65536.

	*/

	switch(decimation){
		case 0 :
			break;

		case 1 : 
			error += rp_AcqSetDecimation(RP_DEC_1);
			break; // Sample rate 125Msps; Buffer time length 131us; Decimation 1.

		case 8 :
			error += rp_AcqSetDecimation(RP_DEC_8);
			break; // Sample rate 15.625Msps; Buffer time length 1.048ms; Decimation 8.

		case 64 :
			error += rp_AcqSetDecimation(RP_DEC_64);
			break; // Sample rate 1.953Msps; Buffer time length 8.388ms; Decimation 64.

		case 1024 : 
			error += rp_AcqSetDecimation(RP_DEC_1024);
			break; // Sample rate 122.070ksps; Buffer time length 134.2ms; Decimation 1024.

		case 8192 :
			error += rp_AcqSetDecimation(RP_DEC_8192);
			break; // Sample rate 15.258ksps; Buffer time length 1.073s; Decimation 8192.

		case 65536 :
			error += rp_AcqSetDecimation(RP_DEC_65536);
			break; // Sample rate 1.907ksps; Buffer time length 8.589s; Decimation 65536.

		default :
			printf("\n Invalid decimation value used. \n");
			printf("Please use 1, 8, 64, 1024, 8192, 65536 and 0 for break \n");
			error = EXIT_FAILURE;
	}

	

	
	//	rp_AcqSetTriggerDelay(0);  // 
	//	rp_AcqStart();

	/* 
	After acquisition is started some time delay is needed in order to acquire fresh samples in to buffer
	Here we have used time delay of one second but you can calculate exact value taking in to account buffer
	length and smaling rate
	*/

	  // PART COPIED FROM THE GENERATOR CODE NEEDS TO BE SYNCED HERE WITH ACQUISITON
  
// THIS WAS REMOVED LAST
  rp_GenTrigger(RP_CH_1);
  //  usleep(500);
   
  //  usleep(30);  // This needs to be changed due to pulse durattion
    
    // Here we need to add code to turn of the switch to measure capacitance!!!!!!!!!!


   
//rp_acq_trig_state_t state = RP_TRIG_STATE_TRIGGERED;
  // -------------------END OF CODE --------------------------

		//sleep(1);  // !!!!TEST IF IS REALY NESSESERY TO HAVE ONE SECOND TO WAIT
		rp_AcqSetTriggerDelay(0);  // 
		//rp_GenTriggerSource(RP_CH_1, RP_GEN_TRIG_SRC_INTERNAL);
		rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW); //RP_TRIG_SRC_NOW

		rp_AcqStart();
		//usleep(30);
		
		//rp_acq_trig_state_t state = RP_TRIG_STATE_TRIGGERED;

		rp_GenOutEnable(RP_CH_1);

		//rp_AcqStart();

		usleep(30);  // This needs to be changed due to pulse durattion
    
    // Here we need to add code to turn of the switch to measure capacitance!!!!!!!!!!


		// Turn off the signal generator! 
    rp_GenOutDisable(RP_CH_1);
    getTao();

    usleep(10);
    rp_AcqStop();

	/*	while(1){
	                rp_AcqGetTriggerState(&state);
	                if(state == RP_TRIG_STATE_TRIGGERED){
	                sleep(1);
	                break;
	                }
	        }*/
//	    sleep(1);
	    rp_AcqGetOldestDataV(RP_CH_1, &buff_size, OLD_buff_ch1);
	    rp_AcqGetOldestDataV(RP_CH_2, &buff_size, OLD_buff_ch2);
	    
	    rp_AcqGetLatestDataV(RP_CH_1, &buff_size, buff_ch1);
	    rp_AcqGetLatestDataV(RP_CH_2, &buff_size, buff_ch2);

 

	    int i;
	    for(i = 0; i < buff_size; i++){
	    	printf("%f\t%f\n", buff_ch1[i], buff_ch2[i]);
	    }

	    int k;
	    for (k = 0; k<100; k++){
	    	printf("0.00\t0.00\n");
	    }

	    int j;
	    for(j = 0; j < buff_size; j++){
	    	printf("%f\t%f\n", OLD_buff_ch1[j], OLD_buff_ch2[j]);
	    }	
	
	   

    // TOLE MORA BIT NA KONCU
	/* Releasing resources */
    
    free(buff_ch1);
	free(buff_ch2);
	free(OLD_buff_ch1);
	free(OLD_buff_ch2);


	// PREGLEJ SITUACIJO!!!
    return(error);

}
