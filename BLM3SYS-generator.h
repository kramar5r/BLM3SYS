/*
#########################################################
#                                                       #
#   BLM3SYS  -> Signal generator variable deffinition	#
#           	HEADER FILE								#
#                                                       #
#   Author: Peter Kramar       Date: 17. 1. 2017	    #
#########################################################
*/

#ifndef generator_H
#define generator_H

// Global variable definition
extern int A_B;		// < DIO2_P on N7
extern int C_D;		// < DIO3_P on N7

// Function definition
void setVoltageClamp();
void setCurrentClamp();
void setIonChannel();
void ContiniusGeneration(float freq, float amp);
void ContiniusPulseGeneration(float freq, float amp);
void disableContGeneration();
void singlePulseGeneration(float amp, float freq, int Nburst);
void dischargePulseGenerator(float amp, float freq);
void singleRampGeneration(float k, float amp);
int *FindResistors(float Vint, float Vout);
int setResistors(float R1, float R2);


#endif