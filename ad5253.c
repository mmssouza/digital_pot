#include "ad5253.h"
#include "i2c.h"

void ad5243_write(uint8_t dev,uint8_t data,uint8_t addr,uint8_t eerdac)
{   
  I2C_start();
  I2C_write(0b01011000);
  I2C_write(addr);
  I2C_write(data);
  I2C_stop();
} 


void ad5243_quick_cmd(char dev,char cmd,char reg)
{
  dev = 0b01011000 | (dev << 1);
  switch (cmd) {
      case LD_EEMEM_RDAC:
      case ST_RDAC_EEMEM:
      case DEC_RDAC_6DB:
      case DEC_RDAC:
      case INC_RDAC_6DB:
      case INC_RDAC:    
          cmd |= reg;
          break;
      default: 
          break;        
  }
  
  I2C_start();
  I2C_write(dev);
  I2C_write(cmd);
  I2C_stop(); 
}
