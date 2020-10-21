
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

void I2C_init (uint32_t clock);
void I2C_wait (void);
void I2C_start (void);
void I2C_repeated_start (void);
void I2C_stop (void);
void I2C_write (uint8_t data);
uint8_t I2C_read (uint8_t ack);


#endif	/* XC_HEADER_TEMPLATE_H */

