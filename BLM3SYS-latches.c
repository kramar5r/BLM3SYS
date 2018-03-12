
/*
#########################################################
#                                                       #
#   BLM3SYS  -> Latches variable deffinition	        #
#           	FUNCTIONS								#
#                                                       #
#   Author: Peter Kramar       Date: 3. 1. 2017	        #
#########################################################

List of Functions:

void OutputPinDefinition()
void UpdatePinStructures()
void writeOnN0()
void writeOnN6()
void writeOnN7()
void WriteAllBus()
int RPLevel(int variable)
void selectSPICSS(int Yn)
void WritOnCssMux()
void setVCAtten()
void setVCAmpl()
void triggerNSpulse()
void getTao()

*/

#include "BLM3SYS-latches.h"

#include "redpitaya/rp.h"
#include <unistd.h>


void OutputPinDefinition(){

    // Defines all digital pins on port P and N on Red Pitaya as output.

	rp_DpinSetDirection(RP_DIO0_P, RP_OUT);
    rp_DpinSetDirection(RP_DIO1_P, RP_OUT);
    rp_DpinSetDirection(RP_DIO2_P, RP_OUT);
    rp_DpinSetDirection(RP_DIO3_P, RP_OUT);
    rp_DpinSetDirection(RP_DIO4_P, RP_OUT);
    rp_DpinSetDirection(RP_DIO5_P, RP_OUT);
    rp_DpinSetDirection(RP_DIO6_P, RP_OUT);
    rp_DpinSetDirection(RP_DIO7_P, RP_OUT);

    rp_DpinSetDirection(RP_DIO0_N, RP_OUT);
    rp_DpinSetDirection(RP_DIO1_N, RP_OUT);
    rp_DpinSetDirection(RP_DIO2_N, RP_OUT);
    rp_DpinSetDirection(RP_DIO3_N, RP_OUT);
    rp_DpinSetDirection(RP_DIO4_N, RP_OUT);
    rp_DpinSetDirection(RP_DIO5_N, RP_OUT);
    rp_DpinSetDirection(RP_DIO6_N, RP_OUT);
    rp_DpinSetDirection(RP_DIO7_N, RP_OUT);

    UpdatePinStructures();
    WriteAllBus(); 

}


void UpdatePinStructures(){
    
    // Update pin values to the port structure

	N0.DIO0=AM_S0;
	N0.DIO1=AM_S1;
	N0.DIO2=AM_S2;
	N0.DIO3=AM_E;
	N0.DIO4=AM_RC_SMA;
	N0.DIO5=SHDN;
	N0.DIO6=PR;
	N0.DIO7=AMP_ATT;

	N6.DIO0=LED0;
	N6.DIO1=LED1;
	N6.DIO2=LED2;
	N6.DIO3=LED3;
	N6.DIO4=LED4;
	N6.DIO5=VM_RC_SMA;
	N6.DIO6=A1;
	N6.DIO7=A0;

	N7.DIO0=EMPTY;
	N7.DIO1=RS232_SEL;
	N7.DIO2=A_B;
	N7.DIO3=C_D;
	N7.DIO4=CC_E;
	N7.DIO5=CC_S2;
	N7.DIO6=CC_S0;
	N7.DIO7=CC_S1;

    SPI_CSS.S2=I3;
    SPI_CSS.S1=I2;
    SPI_CSS.S0=I1;
}


void writeOnN0(){

    // writing port structure to the P port of RedPitaya with CS N0 pin

	rp_DpinSetState(RP_DIO0_P, RPLevel(N0.DIO0));
	rp_DpinSetState(RP_DIO1_P, RPLevel(N0.DIO1));
	rp_DpinSetState(RP_DIO2_P, RPLevel(N0.DIO2));
	rp_DpinSetState(RP_DIO3_P, RPLevel(N0.DIO3));
	rp_DpinSetState(RP_DIO4_P, RPLevel(N0.DIO4));
	rp_DpinSetState(RP_DIO5_P, RPLevel(N0.DIO5));
	rp_DpinSetState(RP_DIO6_P, RPLevel(N0.DIO6));
	rp_DpinSetState(RP_DIO7_P, RPLevel(N0.DIO7));

	// wriitting to latch 
    rp_DpinSetState(RP_DIO0_N, RP_HIGH);
    usleep(1);
    rp_DpinSetState(RP_DIO0_N, RP_LOW);

}


void writeOnN6(){

    // writing port structure to the P port of RedPitaya with CS N6 pin

	rp_DpinSetState(RP_DIO0_P, RPLevel(N6.DIO0));
	rp_DpinSetState(RP_DIO1_P, RPLevel(N6.DIO1));
	rp_DpinSetState(RP_DIO2_P, RPLevel(N6.DIO2));
	rp_DpinSetState(RP_DIO3_P, RPLevel(N6.DIO3));
	rp_DpinSetState(RP_DIO4_P, RPLevel(N6.DIO4));
	rp_DpinSetState(RP_DIO5_P, RPLevel(N6.DIO5));
	rp_DpinSetState(RP_DIO6_P, RPLevel(N6.DIO6));
	rp_DpinSetState(RP_DIO7_P, RPLevel(N6.DIO7));

	// writting to latch 
    rp_DpinSetState(RP_DIO6_N, RP_HIGH);
    usleep(1);
    rp_DpinSetState(RP_DIO6_N, RP_LOW);

}



void writeOnN7(){

    // writing port structure to the P port of RedPitaya with CS N7 pin

	rp_DpinSetState(RP_DIO0_P, RPLevel(N7.DIO0));
	rp_DpinSetState(RP_DIO1_P, RPLevel(N7.DIO1));
	rp_DpinSetState(RP_DIO2_P, RPLevel(N7.DIO2));
	rp_DpinSetState(RP_DIO3_P, RPLevel(N7.DIO3));
	rp_DpinSetState(RP_DIO4_P, RPLevel(N7.DIO4));
	rp_DpinSetState(RP_DIO5_P, RPLevel(N7.DIO5));
	rp_DpinSetState(RP_DIO6_P, RPLevel(N7.DIO6));
	rp_DpinSetState(RP_DIO7_P, RPLevel(N7.DIO7));

	// writing to latch 
    rp_DpinSetState(RP_DIO7_N, RP_HIGH);
    usleep(1);
    rp_DpinSetState(RP_DIO7_N, RP_LOW);

}


void WriteAllBus(){

    // writing port structures to all P ports of RedPitaya

    writeOnN0();
    writeOnN6();
    writeOnN7();
}


int RPLevel(int variable){

    // converting values to RedPitaya values

	if (variable==1) return RP_HIGH;
	else return RP_LOW;
}


void selectSPICSS(int Yn){

    // Selecting CS for SPI BUS

    /*
    Control of the 8:1 MUX circuit 74HC4051D. Truth table:
    Yn      S2      S1      S0      Function
   =========================================================== 
    0       0       0       0       CS_T3
    1       0       0       1       CS_T2
    2       0       1       0       CS_T1
    3       0       1       1       CS_T4
    4       1       0       0       CS_Dig_Pot
    5       1       0       1       xxxx NOT USED xxxx
    6       1       1       0       xxxx NOT USED xxxx
    7       1       1       1       xxxx NOT USED xxxx
    */

    switch(Yn){
        case 0:
            I3=0; I2=0; I1=0;
            break;
        case 1:
            I3=0; I2=0; I1=1; 
            break;
        case 2:
            I3=0; I2=1; I1=0;
            break;
        case 3:
            I3=0; I2=1; I1=1;
            break;
        case 4:
            I3=1; I2=0; I1=0;
            break;
        case 5:
            I3=1; I2=0; I1=1;
            break;
        case 6:
            I3=1; I2=1; I1=0;
            break;
        case 7:
            I3=1; I2=1; I1=1;
            break;
    }
    UpdatePinStructures();
    WritOnCssMux();
}


void WritOnCssMux(){

    // Writing to RP N-Port to setup the 8:1 MUX circuit 74HC4051D

    rp_DpinSetState(RP_DIO1_N, RPLevel(SPI_CSS.S2));
    rp_DpinSetState(RP_DIO2_N, RPLevel(SPI_CSS.S1));
    rp_DpinSetState(RP_DIO3_N, RPLevel(SPI_CSS.S0));
}


void setVCAtten(){

    // Setup of Attenuation

    AMP_ATT=1;   
    UpdatePinStructures();
    writeOnN0();
}


void setVCAmpl(){

    // setup of Amplification 

    AMP_ATT=0;  
    UpdatePinStructures();
    writeOnN0();
}

void triggerNSpulse(){
    // this function triggers nanosecond pulse generator

    rp_DpinSetState(RP_DIO4_N, RPLevel(1));
    usleep(70000);
    // without any pause the width of the generated pulse is 77.6 ms   
    rp_DpinSetState(RP_DIO4_N, RPLevel(0));
}

void getTao(){
    // this function turn on and turns off a switch on the end of the output
    rp_DpinSetState(RP_DIO5_N, RP_HIGH);
    sleep(1);
    rp_DpinSetState(RP_DIO5_N, RP_LOW);
}
