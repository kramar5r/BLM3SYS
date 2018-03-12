/*
#########################################################
#                                                       #
#   BLM3SYS  -> Latches variable deffinition	        #
#           	HEADER FILE								#
#                                                       #
#   Author: Peter Kramar       Date: 3. 1. 2017	        #
#########################################################
*/

#ifndef latches_H
#define latches_H

// N0 - Initial latch variables to be set
extern int AM_S0;  	// < DIO0_P
extern int AM_S1;	// < DIO1_P
extern int AM_S2;	// < DIO2_P
extern int AM_E;		// < DIO3_P
extern int AM_RC_SMA;// < DIO4_P
extern int SHDN;		// < DIO5_P
extern int PR;		// < DIO6_P
extern int AMP_ATT;	// < DIO7_P

// N6 - Initial latch variables to be set
extern int LED0;		// < DIO0_P
extern int LED1;		// < DIO1_P
extern int LED2;		// < DIO2_P
extern int LED3;		// < DIO3_P
extern int LED4;		// < DIO4_P
extern int VM_RC_SMA;// < DIO5_P
extern int A1;		// < DIO6_P
extern int A0;		// < DIO7_P

// N7 - Initial latch variables to be set
extern int EMPTY;	// < DIO0_P
extern int RS232_SEL;// < DIO1_P
extern int A_B;		// < DIO2_P
extern int C_D;		// < DIO3_P
extern int CC_E;		// < DIO4_P
extern int CC_S2;	// < DIO5_P
extern int CC_S0;	// < DIO6_P
extern int CC_S1;	// < DIO7_P


// Port N - NO latches attached
extern int I3;       // < DIO1_N
extern int I2;       // < DIO2_N
extern int I1;       // < DIO3_N

// Initial values before triggering
extern int TTL;      // < DIO4_N
extern int TAO;      // < DIO5_N



struct DIO_P_Port {
	int DIO0;
	int DIO1;
	int DIO2;
	int DIO3;
	int DIO4;
	int DIO5;
	int DIO6;
	int DIO7;
};

struct Dmux8{
    int S2;
    int S1;
    int S0;
};

struct DIO_P_Port N0;
struct DIO_P_Port N6;
struct DIO_P_Port N7;

struct Dmux8 SPI_CSS;


// Function headers

void OutputPinDefinition();
void UpdatePinStructures();
void writeOnN7();
void writeOnN6();
void writeOnN0();
void WriteAllBus();
int RPLevel(int variable);
void selectSPICSS(int Yn);  // TO TEST
void WritOnCssMux();  // TO TEST + DO THE PROGRAMME
void setVCAtten();
void setVCAmpl();
void triggerNSpulse();
void getTao();

#endif
