
#include <xc.h>
#include <stdint.h>
#include "i2c.h"
#define _XTAL_FREQ 4000000 // Para atraso e definição do clock i2c (scl)
void I2C_init (uint32_t clock)
{   
    TRISB0 = 1;
    TRISB1 = 1;
    SSPADD = (_XTAL_FREQ/(4*clock))-1;  // here clock is the BR/clock speed    
    SSPCON1 = 0b00101000;     //first 4 bits I2c master mode , 6th bit is SSPEN enables the scl and sda line
			      //Set/Reset: S, P, SSPxSTAT, WCOL, SSPOV
    SSPCON2 = 0x00; //Reset SEN, PEN (SSPxCON2)
    SSPIF = 0; BCLIF = 0;//Set SSPxIF, BCLxIF
    SSPSTAT |= 0x80;
    
}
 
void I2C_wait (void)
{
    while (!SSPIF);
    SSPIF = 0;
}
 
void I2C_start (void)
{
   SSPCON2bits.SEN = 1; // SEN=1 -> initiate the START condition on SDA and SCL pins   
   while(SSPCON2bits.SEN || (!SSPIF));
   SSPIF = 0;
}
 
void I2C_repeated_start (void)
{
    I2C_wait();
    SSPCON2 |= 0x02; // RSEN=1  -> initiate REPEATED START condition on SDA and SCL pins
    while (!SSPIF & SSPCON2 & 0xFD);
    SSPIF = 0;
}
 
 
void I2C_stop (void)
{
    SSPCON2bits.PEN  = 1; // PEN=1 -> initiate the STOP condition on SDA and SCL pins    I2C_wait();
    while(SSPCON2bits.PEN || (!SSPIF));
}
 
 
void I2C_write (uint8_t data)
{  
    SSPBUF = data;  // load data into SSPBUF register
    while (SSPCON2bits.ACKSTAT || (!SSPIF));
    SSPIF = 0;  
}
 
 
uint8_t I2C_read (uint8_t ack)
{
    uint8_t temp;
    I2C_wait();
    RCEN = 1;    // enable receive mode for I2c 
    I2C_wait();
    temp = SSPBUF;   // load data from Buffer to the temp
    I2C_wait();
    ACKDT = (ack);  // 0-- not ACK , 1-- ACK
    ACKEN = 1;   // Send Acknowledgement
    return temp;
}
