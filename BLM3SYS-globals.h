#ifndef globals_H
#define globals_H

// Global Variables


// N0 - Initial latch variables to be set
int AM_S0=0;  	// < DIO0_P
int AM_S1=0;	// < DIO1_P
int AM_S2=0;	// < DIO2_P
int AM_E=0;		// < DIO3_P
int AM_RC_SMA=0;// < DIO4_P
int SHDN=1;		// < DIO5_P
int PR=1;		// < DIO6_P
int AMP_ATT=0;	// < DIO7_P


// N6 - Initial latch variables to be set
int LED0=0;		// < DIO0_P
int LED1=0;		// < DIO1_P
int LED2=0;		// < DIO2_P
int LED3=0;		// < DIO3_P
int LED4=0;		// < DIO4_P
int VM_RC_SMA=0;// < DIO5_P
int A1=0;		// < DIO6_P
int A0=0;		// < DIO7_P


// N7 - Initial latch variables to be set
int EMPTY=0;	// < DIO0_P
int RS232_SEL=0;// < DIO1_P
int A_B=0;		// < DIO2_P
int C_D=0;		// < DIO3_P
int CC_E=0;		// < DIO4_P
int CC_S2=0;	// < DIO5_P
int CC_S0=0;	// < DIO6_P
int CC_S1=0;	// < DIO7_P


// Port N - NO latches attached
int I3=0;       // < DIO1_N
int I2=0;       // < DIO2_N
int I1=0;       // < DIO3_N

// Initial values before triggering
int TTL=0;      // < DIO4_N
int TAO=0;      // < DIO5_N
                         
#endif                
