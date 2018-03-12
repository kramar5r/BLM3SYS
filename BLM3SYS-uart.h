/*
#########################################################
#                                                       #
#   BLM3SYS  -> UART Communication 						#
#           	HEADER FILE								#
#                                                       #
#   Author: Peter Kramar       Date:  31. 5. 2017       #
#########################################################

*/

#ifndef uart_H
#define uart_H

int uart_init();
int release();
int uart_read(int size);
int uart_write();

void write_pump_stop(char *addr);
void write_pump_dir(char *addr, int dir);
void write_pump_set_diam(char *addr, char *diameter);
void write_pump_set_vol(char *addr, char *volume);
void write_pump_set_rate(char *addr, char *rate);
void write_pump_run(char *addr);


#endif