
#ifndef AD5253_H
#define	AD5253_H

#include <stdint.h>
void ad5243_write(uint8_t dev,uint8_t data,uint8_t addr,uint8_t eerdac);
//void ad5243_rdac_store(char dev,char reg);
//void ad5243_rdac_restore(char dev,char reg);
void ad5243_quick_cmd(char dev,char cmd, char reg);				  	    

#define READ 0x01 
#define WRITE 0xFE

#define CMD 0x80
#define REG 0x7F

#define EE   0x20
#define RDAC 0xDF

// RDAC registers
#define RDAC0  0b00
#define RDAC1  0b01 
#define RDAC2  0b10 
#define RDAC3  0b11

// EEREG registers
#define EEREG0  0
#define EEREG1  1
#define EEREG2  2
#define EEREG3  3
#define EEREG4  4
#define EEREG5  5 
#define EEREG6  6 
#define EEREG7  7
#define EEREG8  8
#define EEREG9  9
#define EEREG10  10
#define EEREG11  11
#define EEREG12  12
#define EEREG13  13
#define EEREG14  14
#define EEREG15  15

// Quick commands (cmd = 1)

// no operation
#define NOOP 0

// load EEMEMx to RDACx
                           
#define LD_EEMEM_RDAC 0b10001000

// store RDACx to EEMEMx

#define ST_RDAC_EEMEM 0b10010000

// RDACx 6 dB decrement 

#define DEC_RDAC_6DB  0b10011000

// All RDACx's 6 dB decrement 

#define DEC_RDAC_6DB_ALL 0b10100000

// one step RDACx decrement 

#define DEC_RDAC 0b10101000

// one step all RDACx's decrement 

#define DEC_RDAC_ALL 0b10110000

// Load all EEMEMx's to RDACx's

#define LD_EEMEM_RDAC_ALL 0b10111000

// RDACx 6 dB increment 

#define INC_RDAC_6DB 0b11000000

// all RDACx's 6 dB increment 

#define INC_RDAC_6DB_ALL 0b11001000

// RDACx one step increment 

#define INC_RDAC 0b11010000

// all RDACx's one step increment 

#define INC_RDAC_ALL 0b11011000

#endif	/* AD5253_H */