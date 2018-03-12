/*
#########################################################
#                                                       #
#   BLM3SYS  -> UART Communication						#
#           	FUNCTION FILE							#
#                                                       #
#   Author: Peter Kramar       Date:  31. 5. 2017       #
#########################################################

List of Functions:

int uart_init();
int release();
int uart_read(int size);
int uart_write();

void write_pump_stop(int fd, char *addr);
void write_pump_dir(int fd, char *addr, int dir);
void write_pump_set_diam(int fd, char *addr, char *diameter);
void write_pump_set_vol(int fd, char *addr, char *volume);
void write_pump_set_rate(int fd, char *addr, char *rate);
void write_pump_run(int fd, char *addr);


static int uart_init()

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART
#include <errno.h>
#include "BLM3SYS-uart.h"

int uart_fd = -1;

int uart_init(){

	uart_fd = open("/dev/ttyPS1", O_RDWR | O_NOCTTY | O_NDELAY);

	if(uart_fd == -1){
		fprintf(stderr, "Failed to open uart.\n");
		return -1;
	}

	struct termios settings;
	tcgetattr(uart_fd, &settings);

	/*  CONFIGURE THE UART
	 *  The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
	 *	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
	 *	CSIZE:- CS5, CS6, CS7, CS8
	 *	CLOCAL - Ignore modem status lines
	 * 	CREAD - Enable receiver
	 *	IGNPAR = Ignore characters with parity errors
	 *	ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
	 *	PARENB - Parity enable
	 *	PARODD - Odd parity (else even) */

	/* Set baud rate - default set to 9600Hz */
	speed_t baud_rate = B9600;

	/* Baud rate fuctions
	 * cfsetospeed - Set output speed
	 * cfsetispeed - Set input speed
	 * cfsetspeed  - Set both output and input speed */

	cfsetspeed(&settings, baud_rate);

	settings.c_cflag &= ~PARENB; /* no parity */
	settings.c_cflag &= ~CSTOPB; /* 1 stop bit */
	settings.c_cflag &= ~CSIZE;
	settings.c_cflag |= CS8 | CLOCAL; /* 8 bits */
	settings.c_lflag = ICANON; /* canonical mode */
	settings.c_oflag &= ~OPOST; /* raw output */
	
	/* Setting attributes */
	tcflush(uart_fd, TCIFLUSH);
	tcsetattr(uart_fd, TCSANOW, &settings);

	return 0;
}

int uart_read(int size){

	/* Read some sample data from RX UART */
	
	/* Don't block serial read */
	fcntl(uart_fd, F_SETFL, FNDELAY); 

	while(1){
		if(uart_fd == -1){
			fprintf(stderr, "Failed to read from UART.\n");
			return -1;
		}

		unsigned char rx_buffer[size];

		int rx_length = read(uart_fd, (void*)rx_buffer, size);

		if (rx_length < 0){

			/* No data yet avaliable, check again */
			if(errno == EAGAIN){
				fprintf(stderr, "AGAIN!\n");
				continue;
			/* Error differs */
			}else{
				fprintf(stderr, "Error!\n");
				return -1;
			}

		}else if (rx_length == 0){
			fprintf(stderr, "No data waiting\n");
		/* Print data and exit while loop */
		}else{
			rx_buffer[rx_length] = '\0';
			printf("%i bytes read : %s\n", rx_length, rx_buffer);
			break;

		}
	}

	return 0;
}

int uart_write(char *data){

	/* Write some sample data into UART */
	/* ----- TX BYTES ----- */
	int msg_len = strlen(data);

	int count = 0;
	char tx_buffer[msg_len+1];

	strncpy(tx_buffer, data, msg_len);
	tx_buffer[msg_len++] = 0x0a; //New line numerical value

	if(uart_fd != -1){
		count = write(uart_fd, &tx_buffer, (msg_len));
	}
	if(count < 0){
		fprintf(stderr, "UART TX error.\n");
		return -1;
	}
	
	return 0;
}


 int release(){

	tcflush(uart_fd, TCIFLUSH);
	close(uart_fd);

	return 0;
}

/* ---------------  Functions to control the pumps ----------------------*/


/* function to write the stp command of the pump on the serial port */
void write_pump_stop(char *addr)
{
	char stops[9]; 
	char *stp = " stp\r\n";

	strcpy (stops, addr);
	strcat (stops, stp);

	write(uart_fd, stops, strlen(stops));        /// sent the stop command to the pump the final string is "\0 stp\r\n" for pump 0
	tcdrain (uart_fd);               /// coded on 7 characters

	tcflush (uart_fd, TCIFLUSH);
}


/* function to set the direction of pumping*/
void write_pump_dir(char *addr, int dir)
{

	char dirs [12];
	char *direct = " dir";

	char *end = "\r\n";

	strcpy(dirs, addr);
    strcat(dirs, direct);
    if (dir == 0)
    {
    	strcat(dirs, "wdr");
    }else{
    	strcat(dirs, "inf");
    }
    strcat(dirs, end);

    write(uart_fd, dirs, strlen(dirs));
    tcdrain(uart_fd);

    tcflush(uart_fd, TCIFLUSH);
}

/* function to write diameter of syringe parameter in the pump*/
void write_pump_set_diam(char *addr, char *diameter)
{
	char diams[13];

	char *comd = " dia";
    char *end = "\r\n";

    strcpy (diams, addr);
    strcat (diams, comd);
    strcat (diams, diameter);
    strcat (diams, end);
    
		write(uart_fd, diams, strlen(diams));
		
}

/* function to write volume to pump parameter in the pump*/
void write_pump_set_vol(char *addr, char *volume)
{
	char vols[13];

	char *comv = " vol";
    char *end = "\r\n";

    strcpy (vols, addr);
    strcat (vols, comv);
    strcat (vols, volume);
    strcat (vols, end);
    
		write(uart_fd, vols, strlen(vols));
		
}

/* function to write rate of pumping parameter in the pump*/
void write_pump_set_rate(char *addr, char *rate)
{
	char rates[13];

	char *comr = " rat";
	char *unit = "mm";
    char *end = "\r\n";

    strcpy (rates, addr);
    strcat (rates, comr);
    strcat (rates, rate);
    strcat (rates, unit);    
    strcat (rates, end);
    
		write(uart_fd, rates, strlen(rates));
		
}

/* function to start the work of the pump*/
void write_pump_run(char *addr)
{
	char runs[9]; 
	char *run = " run\r\n";

	strcpy (runs, addr);
	strcat (runs, run);

	write(uart_fd, runs, strlen(runs));                        /// send run command to the pump 
	tcdrain(uart_fd);

	tcflush(uart_fd, TCIFLUSH);
}
