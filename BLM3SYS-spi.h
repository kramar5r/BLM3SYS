/*
#########################################################
#                                                       #
#   BLM3SYS  -> SPI communication protocol				#
#           	HEADER FILE								#
#                                                       #
#   Author: Peter Kramar       Date: 15. 2. 2017        #
#########################################################
*/


#ifndef spi_H
#define spi_H

#define ARRAY_SIZE(a) (sizeof(a)/sizeof((a)[0]))

// Global variable definition
extern int SHDN;		// < DIO5_P
extern int PR;			// < DIO6_P


// Function definition
void selectSPICSS(int Yn);
int init_spi();
int release_spi();
void pabort(const char *s);
void transfer(int spi_fdX, float VINT, float VOUT);
int setVCResistorsValue(float VINT, float VOUT);

int write_spi(char *write_buffer, int size);


#endif