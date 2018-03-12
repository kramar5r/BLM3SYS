/*
#########################################################
#                                                       #
#   BLM3SYS  -> Signal generator operation and command  #
#           	FUNCTION FILE							#
#                                                       #
#   Author: Peter Kramar       Date: 17. 1. 2017        #
#########################################################

List of Functions:

void setVoltageClamp();
void setCurrentClamp();
void setIonChannel();
void ContiniusGeneration(float freq, float amp);
void disableContGeneration();
void singlePulseGeneration(float amp, float freq, int Nburst);
void singleRampGeneration(float k, float amp);
int *FindResistors(float Vint, float Vout);


*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "redpitaya/rp.h"


#include "BLM3SYS-leds.h"
#include "BLM3SYS-latches.h"
#include "BLM3SYS-generator.h"
#include "BLM3SYS-spi.h"




#include <math.h>
#define M_PI 3.14159265358979323846


// Selection of Voltage clamp -> VC, Current Clamp CC, and Ion Channel IC

void setVoltageClamp(){
	
	A_B = 0;
	C_D = 0;
	UpdatePinStructures();
    writeOnN7();

} 

void setCurrentClamp(){

	A_B = 0;
	C_D = 1;
	UpdatePinStructures();
    writeOnN7();

}

void setIonChannel(){

	A_B = 1;
	C_D = 0;
	UpdatePinStructures();
    writeOnN7();

}


void ContiniusGeneration(float freq, float amp){   // WORK HERE TO PROGRAM ALL AT HOME!!!  FUNCTION NEED TO HAVE INPUT ARGUMENTS TO SETUP
                                // generation...
	// Generating frequency 
	rp_GenFreq(RP_CH_1, freq);
    // Generating amplitude 
	rp_GenAmp(RP_CH_1, amp);
    // Generating wave form 
	rp_GenWaveform(RP_CH_1, RP_WAVEFORM_SINE);
	
    // HERE MISSING:
    /*
        Amplification has to be set up. But this will be done when output will be tested.

    */


	// Enable channel 
	rp_GenOutEnable(RP_CH_1);

}

void ContiniusPulseGeneration(float freq, float amp){
    // Generating frequency 
    rp_GenFreq(RP_CH_1, freq);
    // Generating amplitude 
    rp_GenAmp(RP_CH_1, amp);
    // Generating wave form 
    rp_GenWaveform(RP_CH_1, RP_WAVEFORM_SQUARE);
    
    // HERE MISSING:
    /*
        Amplification has to be set up. But this will be done when output will be tested.

    */


    // Enable channel 
    rp_GenOutEnable(RP_CH_1);
}

void disableContGeneration(){

	// Turn off the signal generator!
    rp_GenOutDisable(RP_CH_1);
}


void singlePulseGeneration(float amp, float freq, int Nburst){  // THIS IS CHALLENGE 1
	
	int i;
	int buff_size = 16384;

	float *x = (float *)malloc(buff_size * sizeof(float));

	for(i = 1; i < buff_size/2; i++){
		x[i]=1;
    }

    for (i = buff_size/2+1; i < buff_size-1; i++){
    	x[i]=0;
    }

    x[0]=0;
    x[16384]=0;

    rp_GenWaveform(RP_CH_1, RP_WAVEFORM_ARBITRARY);
    rp_GenArbWaveform(RP_CH_1, x, buff_size);
    rp_GenFreq(RP_CH_1, freq);  // parameter added
    rp_GenAmp(RP_CH_1, amp);    // parameter added
    rp_GenMode(RP_CH_1, RP_GEN_MODE_BURST);
    rp_GenBurstCount(RP_CH_1, Nburst);
    rp_GenBurstRepetitions(RP_CH_1, 1); // parameter added
    rp_GenBurstPeriod(RP_CH_1, 10000);
    rp_GenTrigger(1);
    usleep(500);
    rp_GenOutEnable(RP_CH_1);
    usleep(500);
	// Turn off the signal generator!
    rp_GenOutDisable(RP_CH_1);

        
}

void dischargePulseGenerator(float amp, float freq){
    int i;
    int buff_size = 16384;

    float *x = (float *)malloc(buff_size * sizeof(float));

    for(i = 1; i < buff_size; i++){
        x[i]=1;
    }

    x[0]=0;
    x[16384]=0;

    rp_GenWaveform(RP_CH_1, RP_WAVEFORM_ARBITRARY);
    rp_GenArbWaveform(RP_CH_1, x, buff_size);
    rp_GenFreq(RP_CH_1, freq);  // parameter added
    rp_GenAmp(RP_CH_1, amp);    // parameter added
    rp_GenMode(RP_CH_1, RP_GEN_MODE_BURST);
    rp_GenBurstCount(RP_CH_1, 1);
    rp_GenBurstRepetitions(RP_CH_1, 1); // parameter added
    rp_GenBurstPeriod(RP_CH_1, 10000);

    /* THIS PART WILL BE EXECUTED IN THE MAIN FUNCTION
    rp_GenTrigger(RP_CH_1);
    usleep(500);
    rp_GenOutEnable(RP_CH_1);
    usleep(30);  // This needs to be changed due to pulse durattion
    
    // Here we need to add code to turn of the switch to measure capacitance!!!!!!!!!!


    // Turn off the signal generator! 
    rp_GenOutDisable(RP_CH_1); 

    */
}


	

void singleRampGeneration(float k, float amp){

	float frq;
	float waitingBeforeDisable;

	waitingBeforeDisable = ceil(amp/k*1000);
	frq = 1000 * k / amp;
	
	/*
		Setting up the waveform in the function rp_GenWaveform()
		RP_WAVEFORM_SINE 		Wave form sine.
		RP_WAVEFORM_SQUARE 		Wave form square.
		RP_WAVEFORM_TRIANGLE 	Wave form triangle.
		RP_WAVEFORM_RAMP_UP 	Wave form sawtooth (/|)
		RP_WAVEFORM_RAMP_DOWN 	Wave form reversed sawtooth (|)
		RP_WAVEFORM_DC 			Wave form dc.
		RP_WAVEFORM_PWM 		Wave form pwm.
		RP_WAVEFORM_ARBITRARY 	Use defined wave form.  // something has to be done before...
	*/

	// setup the waveform of generated signal
	rp_GenWaveform(RP_CH_1, RP_WAVEFORM_RAMP_UP);

	/* generating RAMP signal for Planar lipid bilayer experiments
	
		Slope k is defined as: k=A*f/1000 
		Therefore if A is fixed or defined in advance (for this test A = 1.0 V) 
		the frequency is calculated:

		f = 1000 * k / A   where A = 1 V

		Here is table for common frequencies used in previous Experiments:
		
			k [kV/s]  |   f [Hz]
		-----------------------------
			  4.8	  |   4800
			  5.5	  |	  5500
			  7.8	  |   7800
			 11.5	  |  11500
			 16.7	  |  16700
			 21.6	  |  21600
			 48.1 	  |  48100
		-----------------------------
	*/
	
	// Generating frequency 
	rp_GenFreq(RP_CH_1, frq);
    
    // Generating amplitude 
	rp_GenAmp(RP_CH_1, amp);
	
	/*
	mode	Type of signal generation (CONTINUOUS, BURST, STREAM).
	RP_GEN_MODE_CONTINUOUS, RP_GEN_MODE_BURST, RP_GEN_MODE_STREAM
	 */

	// Mode Burst, Single 
	rp_GenMode(RP_CH_1, RP_GEN_MODE_BURST);   
	// Number of Bursts
	rp_GenBurstCount(RP_CH_1, 1);

	rp_GenTrigger(1);
    usleep(10);
	rp_GenOutEnable(RP_CH_1);	

	// MANJKA USLEEP
	

	usleep(waitingBeforeDisable);
	// Turn off the signal generator!
    rp_GenOutDisable(RP_CH_1);

}

	

int *FindResistors(float Vint, float Vout){

	// Sample data THIS TWO LINES WILL BE DELETED AFTER THE RIGHT CODE AND DATA WILL APEAR
    //uint16_t data = {0xff80,0xEF80};
	/*  
	uint8_t iR1=255;
    uint8_t iR2=128;
    uint8_t *data1 = &iR1;
    uint8_t *data2 = &iR2;
    */	

    int *p;
    p = malloc(4*sizeof(int));
    float RA=0,RB=0;

    if ((Vout>0.4) && (Vout<=0.99)){
        printf("Attenuation of the signal [1] \n");
        RB=10;
        RA=RB*((Vint/Vout)-1);
        setVCAtten();
    }

    if ((Vout>=0.06) && (Vout<=0.4)){
        printf("Attenuation of the signal [2]\n");
        RB=1;
        RA=RB*((Vint/Vout)-1);
        setVCAtten();
    }

    if ((Vout>=0.03) && (Vout<0.06)){
    	printf("Attenuation of the signal [3]\n");
        RB=0.6;
        RA=RB*((Vint/Vout)-1);
        setVCAtten();
    }

    if ((Vout>0.004) && (Vout<0.003)){
    	printf("Attenuation of the signal [4]\n");
        RB=0.060;
        RA=RB*((Vint/Vout)-1);
        setVCAtten();
    }

    if ((Vout>=1.0099) && (Vout<=2.5)){
        printf("Amplification of the signal [1]\n");
        RB=10;
        RA=RB*((Vout/Vint)-1);
        setVCAmpl();
    }

    if ((Vout>2.5) && (Vout<=4.5)){
        printf("Amplification of the signal [2]\n");
        RB=5;
        RA=RB*((Vout/Vint)-1);
        setVCAmpl();
    }
    
    int DA=0,DB=0;
    float RAB=20,RW=0.06;
    int Ca=0,Na=0,Cb=0,Nb=0;

    DA = floor((255-((RA-RW)/RAB)*255)+0.5); 
    DB = floor((((RB-RW)/RAB)*255+0.5));

    printf("\n DA = %d  !  DB = %d  \n DA = %x  !  DB = %x  \n", DA, DB, DA, DB);

    printf("\n -opa-- %d\n",DA&(1<<0));
    printf("\n -opb-- %d\n",DB&(1<<0));

    if(DA&(1<<0)){
    	Na = 128;
    }
    else{
    	Na = 0;
    }

    if(DB&(1<<0)){
    	Nb = 128;
    }
    else{
    	Nb = 0;
    }

    Ca = DA >> 1;
    Cb = DB >> 1;
    Cb = Cb | 128;

    p[0]=Ca;
	p[1]=Na;
	p[2]=Cb;
	p[3]=Nb;

    printf("\n %d %d !!! %d %d \n", Ca, Na, Cb, Nb);


	return p;
}

/*
int setResistors(float R1, float R2){
    
    float RAB = 20.0;
    float RW = 0.06;

    int *p;
    p = malloc(4*sizeof(int));

    int DA=0;
    int DB=0;
    int Ca=0;
    int Na=0;
    int Cb=0;
    int Nb=0;

    // Calculate the digital values
    // Updated June 26, 2017 
    DA = floor(256-(256*(R1-RW)/RAB));
    DB = floor(256-(256*(R2-RW)/RAB));

    if(DA&(1<<0)){
        Na = 128;
    }
    else{
        Na = 0;
    }

    if(DB&(1<<0)){
        Nb = 128;
    }
    else{
        Nb = 0;
    }

    Ca = DA >> 1;
    Cb = DB >> 1;
    Cb = Cb | 128;

    p[0]=Ca;
    p[1]=Na;
    p[2]=Cb;
    p[3]=Nb;

    // tukaj imamo nafilan buffer z vrednostmi upora sedaj jih moramo vnesti preko SPI
    printf("\n %d %d !!! %d %d \n", Ca, Na, Cb, Nb); 

    extern int spi_fd;

    if(init_spi() < 0){
        printf("Initialization of SPI failed. Error: %s\n", strerror(errno));
    return -1;
    }

    int ret;
    int K=0;
    uint8_t tx[2]={0};
    for (K=0; K<2; K++){  
        if (K==0){
            tx[0] = p[0]; 
            tx[1]= p[1];
        }

        if (K==1){
            tx[0] = p[2]; 
            tx[1]= p[3];
        }


        printf("\nK: %d Data %d %d \n", K, tx[0], tx[1]);

        uint8_t rx[ARRAY_SIZE(tx)] = {0, };
        struct spi_ioc_transfer tr = {
                .tx_buf = (unsigned long)tx,  
                .rx_buf = (unsigned long)rx,
                .len = ARRAY_SIZE(tx),    
                //.tx_nbits = 8,
                //.delay_usecs = delay,
                .speed_hz = spi_speed,
                .bits_per_word = spi_bits,
                .cs_change = 1,
        };

        ret = ioctl(spi_fdX, SPI_IOC_MESSAGE(1), &tr);
        
        printf("RETURN: %d",ret);

        if (ret == 1)
            pabort("can't send spi message");
    }

    close(spi_fd);
    
    if(release_spi() < 0){
        printf("Relase of SPI resources failed, Error: %s\n", strerror(errno));
    return -1;
    }

    return 0;
}


*/






