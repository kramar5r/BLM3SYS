/*
#########################################################
#                                                       #
#   BLM3SYS  -> Acquire signal and measurement			#
#           	HEADER FILE								#
#                                                       #
#   Author: Peter Kramar       Date:  9. 3. 2017        #
#########################################################

List of Functions:


*/

#ifndef acquire_H
#define acquire_H

// Global variable definition
extern int VM_RC_SMA;
extern int A1;
extern int A0;

extern int AM_RC_SMA;
extern int AM_S0;
extern int AM_S1;
extern int AM_S2;
extern int AM_E;

/*
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
*/

// Function definition
int ContiniousAcquire(int decimation);
int setAmpermeter(int ampermeterAmp, int probeDivAm);
float measureCurrent(float currentRange);
int setVoltmeter(int voltmeterAmp, int probeDiv);
float measureVoltage(float setUinPP);
int SingleAcquire(int decimation);


#endif