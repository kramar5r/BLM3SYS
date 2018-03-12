/*
#########################################################
#                                                       #
#   BLM3SYS  -> testUART	 	           		        #
#      The programme that is compiled and tested 		#
#      as a UART communication protocol on RedPitaya    #
#														#
#   Author: Peter Kramar       Date: 31. 5. 2017        #
#########################################################
*/

#include "redpitaya/rp.h"
#include "BLM3SYS-globals.h"
#include "BLM3SYS-uart.h"
#include "BLM3SYS-latches.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART
#include <errno.h>

extern int RS232_SEL;


/// DODAJ TA DEL ZA INICIALIZACIJO REDPITAYE!!!!!!
int main(int argc, char *argv[]){

	char *data = NULL;
	int port = 0;

	if (argc>1){
		data = argv[1];
		port = atof(argv[2]);	
	}
	else{
		printf("Error on arguments");
	}
	

	if (rp_Init() != RP_OK) {
        fprintf(stderr, "Red Pitaya API init failed!\n");
        return EXIT_FAILURE;
    }

    // All pin startup definition
    OutputPinDefinition(); 
	UpdatePinStructures();
    WriteAllBus();

    // settup the port number
	if(port==2){
		RS232_SEL=1;
		printf("Port 2\n");
	}
	else{
		RS232_SEL=0;
		printf("Port 1\n");
	}
	UpdatePinStructures();
	writeOnN7();


	/* uart init */
	if(uart_init() < 0){
		printf("Uart init error.\n");
		return -1;
	}

	/* Sample write */
	if(uart_write(data) < 0){
		printf("Uart write error\n");
		return -1;
	}

	/* Sample read 
	if(uart_read(strlen(data)) < 0){
		printf("Uart read error\n");
		return -1;
	}
	*/

	/* CLOSING UART */
	release();

	rp_Release();

	return EXIT_SUCCESS;
}