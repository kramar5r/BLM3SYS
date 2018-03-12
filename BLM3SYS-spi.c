/*
#########################################################
#                                                       #
#   BLM3SYS  -> SPI communication protocol				#
#           	FUNCTION FILE							#
#                                                       #
#   Author: Peter Kramar       Date: 15. 2. 2017        #
#########################################################

List of Functions:

void selectSPICSS(int Yn);
static int init_spi();
static int release_spi();
static void pabort(const char *s);
static void transfer(int spi_fd, float VINT, float VOUT);
int setVCResistorsValue(float VINT, float VOUT);


*/

#include "BLM3SYS-spi.h"
#include "BLM3SYS-latches.h"
#include "BLM3SYS-generator.h"
#include "redpitaya/rp.h"

#include <linux/spi/spidev.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
//#include <fstream.h>


int spi_fd = -1;

int init_spi(){

    /* MODES: mode |= SPI_LOOP; 
     *        mode |= SPI_CPHA; 
     *        mode |= SPI_CPOL; 
     *        mode |= SPI_LSB_FIRST; 
     *        mode |= SPI_CS_HIGH; 
     *        mode |= SPI_3WIRE; 
     *        mode |= SPI_NO_CS; 
     *        mode |= SPI_READY;
     *
     * multiple possibilities possible using | */

    static const char *spi_device = "/dev/spidev1.0";
    static uint8_t spi_mode = 0;
   
    static uint8_t spi_bits = 8;
    static uint32_t spi_speed = 1000;  // SPI bus speed
	
	selectSPICSS(4);

    /* Opening file stream */
    spi_fd = open(spi_device, O_RDWR | O_NOCTTY);

    if(spi_fd < 0){
        printf("Error opening spidev0.1. Error: %s\n", strerror(errno));
        return -1;
    }

    /* Setting mode (CPHA, CPOL) */
    if(ioctl(spi_fd, SPI_IOC_WR_MODE, &spi_mode) < 0){
        printf("Error setting SPI_IOC_RD_MODE. Error: %s\n", strerror(errno));
        return -1;
    }

    /* Setting bits per word */
    if(ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bits) < 0){
        printf("Error setting SPI_IOC_WR_BITS_PER_WORD. Error: %s\n", strerror(errno));
        return -1;
    }


    if(ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed) < 0){
        printf("Error setting SPI_IOC_WR_MAX_SPEED_HZ. Error: %s\n", strerror(errno));
        return -1;
    }
       
    return 0;
}

int release_spi(){

    /* Release the spi resources */
    close(spi_fd);

    return 0;
}


void pabort(const char *s){
        perror(s);
        abort();
}



void transfer(int spi_fdX, float VINT, float VOUT){

	static uint8_t spi_bits = 8;
	static uint32_t spi_speed = 1000;
    //unsigned char tab[4]= {0};
    int *tab;
	int i =0;
    tab = malloc(4*sizeof(int));
    tab = FindResistors(VINT,VOUT);
	
	for(i=0;i<4;i++) printf("%d: %d --- %x \n",i,tab[i],tab[i]);    // Tale vrstica naredi izpis za izračunane komponente
        
    int ret;
    int K=0;
    uint8_t tx[2]={0};
    for (K=0; K<2; K++){  
    	if (K==0){
        	tx[0] = tab[0]; 
            tx[1]= tab[1];
        }

        if (K==1){
        	tx[0] = tab[2]; 
            tx[1]= tab[3];
        }


        printf("\nK: %d Data %d %d \n", K, tx[0], tx[1]);

        uint8_t rx[ARRAY_SIZE(tx)] = {0, };
        struct spi_ioc_transfer tr = {
                .tx_buf = (unsigned long)tx,  
                .rx_buf = (unsigned long)rx,
                .len = ARRAY_SIZE(tx),    
                //.tx_nbits = 8,
                //.delay_usecs = delay,
                .speed_hz = spi_speed,
                .bits_per_word = spi_bits,
                .cs_change = 1,
        };

        ret = ioctl(spi_fdX, SPI_IOC_MESSAGE(1), &tr);
        
        printf("RETURN: %d",ret);

        if (ret == 1)
        	pabort("can't send spi message");
        
        for (ret = 0; ret < ARRAY_SIZE(tx); ret++){
        	if (!(ret % 6))
        		printf("%d \n", ret);
                printf("%.2X ", tx[ret]);  // tale vrstica naredi HEXa izpis kode, za potenciometer al kwa moram pogledat
        }
        puts("");

    }
}



/* Read data from the SPI bus */

/* Write data to the SPI bus */

int write_spi(char *write_buffer, int size){

    // DODANO EXSTRA Odprtje SPI KOMUNIKACIJE
    if(init_spi() < 0){
        printf("Initialization of SPI failed. Error: %s\n", strerror(errno));
    return -1;
    }
    // KONEC DODATKA

    int write_spi = write(spi_fd, write_buffer, sizeof(write_buffer));

    if(write_spi < 0){
        printf("Failed to write to SPI. Error: %s\n", strerror(errno));
        return -1;
    }

    // ŠE ZAPRTJE TA DRUZGA SPI
    close(spi_fd);
    
    if(release_spi() < 0){
        printf("Relase of SPI resources failed, Error: %s\n", strerror(errno));
    return -1;
    }
    // KONEC ZAPRTJA SPI


    return 0;
}
    

int setVCResistorsValue(float VINT, float VOUT){

    if(init_spi() < 0){
    	printf("Initialization of SPI failed. Error: %s\n", strerror(errno));
    return -1;
    }

    //static const char *spi_device = "/dev/spidev1.0";
    //int spi_fd = open(spi_device, O_RDWR | O_NOCTTY);
    transfer(spi_fd, VINT, VOUT);
    close(spi_fd);
    
    if(release_spi() < 0){
    	printf("Relase of SPI resources failed, Error: %s\n", strerror(errno));
    return -1;
    }

    return 0;

}


