/*
#########################################################
#                                                       #
#   BLM3SYS  -> testPUMP	 	           		        #
#      The pump programme that is compiled and tested 	#
#      as a UART communication protocol on RedPitaya    #
#														#
#   Author: Peter Kramar       Date:  6. 6. 2017        #
#########################################################
*/

#include "redpitaya/rp.h"
#include "BLM3SYS-globals.h"
#include "BLM3SYS-latches.h"
#include "BLM3SYS-uart.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART
#include <errno.h>

void usage();


const char *g_argv0 = NULL;
extern int RS232_SEL;


int main(int argc, char *argv[]){


	if (rp_Init() != RP_OK) {
        fprintf(stderr, "Red Pitaya API init failed!\n");
        return EXIT_FAILURE;
    }

    // All pin startup definition
    OutputPinDefinition(); 
	UpdatePinStructures();
    WriteAllBus();

    // Port setup. Due to the problems now port is selected as PORT 2 for pumps
	RS232_SEL=1;
	UpdatePinStructures();
	writeOnN7();

    g_argv0 = argv[0];

	if(argc < 6)
	{
		fprintf(stderr, "error : not enough input arguments in %s \n", g_argv0);
		usage();
		return EXIT_FAILURE;
	}

	/*parsing address of the pump */
	char addr[3];
	strcpy(addr, argv[1]);
	int address = atoi(argv[1]);

	/*parsing diameter (in mm) of the seringues argument */
	char diameter[3];
	strcpy(diameter, argv[2]);
	double diam = strtod(argv[2], NULL);
	
	/* parsing volume pumping argument */
	char volume[5];
	strcpy(volume, argv[3]);
	double vol = strtod(argv[3], NULL);

	/* parsing rate of pumping */
	char rate[2];
	strcpy(rate, argv[4]);
	double rat = strtod(argv[4], NULL);
	
	/* parsing direction of pumping*/
	int dir;                           ///direction of pumping, 0 : withdraw, 1 : infuse.
	if (strcmp(argv[5], "wdr") == 0) {
		dir = 0;                       
	}
	if (strcmp(argv[5], "inf") == 0) {
		dir = 1;
	}


	/* 
		This part of the code will just check if 
		the values asking by the user are in the right range 
		for each parameters
	*/

	/* checking the address of the pump */
	if((address < 0) || (address > 99))
	{
		printf("error address of the pump is not valid\n");
		usage();
		return EXIT_FAILURE;
	}

	/* checking diameter value */
	if((diam < 0.0 ) || (diam > 50))
	{
		printf("error diameter of the seringues not valid\n");
		usage();
		return EXIT_FAILURE;
	}

	/* checking volume to pump value */
	if((vol < 0.0) || (vol > 50000))
	{
		printf("error invalid volume\n");
		usage();
		return EXIT_FAILURE;
	}

	/* checking rate of pumping value */
	if(rat < 0.0)
	{
		printf("error invalide rate of pumping\n");
		usage();
		return EXIT_FAILURE;
	}

	/* testing if direction of pumping asking is right */
	if ((strcmp(argv[5], "wdr") != 0) && (strcmp(argv[5], "inf") != 0))
	{
		printf("error invalide direction, wdr : withdraw, inf : infuse \n");
		usage();
		return EXIT_FAILURE;
	}

	/* OPENING UART */
	if(uart_init() < 0){
		printf("Uart init error.\n");
		return -1;
	}



	
    for (int i = 0; i < 15; i++){
    	printf("%2d: ", i);
		switch(i){
			case 0:
            	printf("   parameters of serial communication set\n");
                break;
            case 5:
                write_pump_dir(addr, dir);
                printf("   direction set <%s> on pump %s \n", argv[5], addr);
                break;
            case 10:
				write_pump_set_vol(addr, volume);
            	printf("   volume %s set on pump %s \n", volume, addr);
            	break;
            case 11:
            	write_pump_set_diam(addr, diameter);
            	printf("   syringes diameter of %s was set on pump %s\n", diameter, addr);
            	break;
            case 12:
            	write_pump_set_rate(addr, rate);
            	printf("   rate of pumping set %s for pump %s \n", rate, addr);
            	break;
            case 13:
            	write_pump_dir(addr, dir);
            	printf("   direction set <%s> on pump %s \n", argv[5], addr);
            	break;
            case 14:
                write_pump_run(addr);
                printf("   RUN command for pump %s sent\n", addr);
                break;
            default:
                write_pump_stop(addr);
                printf("   Pump %s was stopped\n", addr);
                break;
        }
    }
    






	/* CLOSING UART */
	release();

	rp_Release();

	return EXIT_SUCCESS;
}


void usage() {

    const char *command_line_format =
        "\n"
        "Usage: %s   address diameter volume rate <direction>\n"
        "\n"
        "\taddress           Address of the pump [0-9], if address asking is > to 9, adress will set to 0.\n"
        "\tdiameter          Diameter of the syringe [0 - ]..\n"
        "\tvolume            Volume to pump [0 - ].\n"
        "\trate              Rate of pumping [0 - ].\n"
        "\tdirection         Direction of pumping [inf, wdr]:  inf->infuse / wdr->withdraw.\n"
        "\n";

    fprintf( stderr, command_line_format, g_argv0);
}