/*
 * File:   main.c
 * Author: marcelo
 *
 * Created on 15 de Outubro de 2020, 15:40
 */
// CONFIG1L
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = XT_XT // Oscillator Selection bits (Internal oscillator, XT used by USB (INTXT))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = ON         // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF   // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)


#include <xc.h>
#include "i2c.h"
#include "ad5253.h"

#define _XTAL_FREQ 4000000
void Init(void);
void UI_Manager(void);
void DPOT_Manager(void);
void W_Updt(void);

#define UI_CMD_NULL 0
#define UI_CMD_INC  1
#define UI_CMD_DEC  2
#define UI_SEL_CMD  3

uint8_t ui_keys = 0xFF;
uint8_t led_ui = 0x10;
uint8_t task = 0;
uint8_t w_idx = 0;
uint8_t w_pos[] = {127,127,127,127};
uint8_t tmr_ticks = 0;
uint8_t cnt_5ms = 0;
uint8_t ui_cmd = UI_CMD_NULL;



void __interrupt(high_priority) ISR(void){
    
    if(TMR2IF){
     TMR2IF = 0;
     //@ 1 ms
     if (++tmr_ticks > 4) {   // inrementa a cada 1 ms 
         tmr_ticks = 0;
         cnt_5ms++;
         ui_keys = ~(PORTC | 0xF8);
         PORTC = ~led_ui;
     }
    }
}

void Init(void) {
  T2CON = 0b00010101;  // 4 us por contagem e 4 estouros / interrupção
  PR2 = 0x53;  // 1 ms para o estouro do TMR2
  TMR2IE = 1;   // Interrupção do TIMER 2
  IPEN = 0; 
  GIEH = 1; 
  GIEL = 1;
  PORTC = 0b11100000;
  TRISC = 0b00001111;
  I2C_init(100000);
  //W_Updt();
  PORTDbits.RD0 = 0;
  TRISDbits.TRISD0 = 0;
}

void main(void) {
  
  Init();
  
  while(1) {
      
   if (cnt_5ms) {
       cnt_5ms = 0;   
   } else 
       continue;
   
   PORTDbits.RD0 ^= 1;
   
   switch (task) {
      case 0:
          DPOT_Manager();
          task = 1;
          break;
       case 1:
          UI_Manager();    
          task = 0;
          break;
      default:
          task = 0;
   }                
  }
}

  void UI_Manager(void) {
      
      static uint8_t cnt = 0;
      static uint8_t keys_ant = 0;

      uint8_t keys = 0;
      // @ 10 ms
      if (ui_keys == keys_ant) {
          cnt++;
          if (cnt > 15) {
              keys = ui_keys;
              cnt = 0;
          }
      } 
      else cnt = 0;  

      keys_ant = ui_keys;
          
      if (keys && (!ui_cmd)) {
          
       uint8_t msk = 1;
       
       while (msk) 
        switch (keys & msk) {
           case 0x00:
               msk <<= 1;
               if (msk & 0x08) 
                 msk = 0;
               break;
           case 0x01:
               led_ui = led_ui & 0x80 ? 0x10 : led_ui << 1;
               ui_cmd = UI_SEL_CMD;
               msk = 0;
               break;
           case 0x02:
               ui_cmd = UI_CMD_INC;
               msk = 0;
               break;
           case 0x04:
               ui_cmd = UI_CMD_DEC;
               msk = 0;
               break;
            default:
                msk = 0;
        }
    }
 }   
  
 void DPOT_Manager(void){
   static unsigned char cnt = 0;
   // @ 10 ms
   if (++cnt < 3) 
    return;     
   else
    cnt = 0;   
   
   switch (ui_cmd) {
           case UI_SEL_CMD:
               w_idx++;
               w_idx &= 0x03;   
               break;
           case UI_CMD_INC:
               w_pos[w_idx]++;
               //ad5243_write(0,w_pos[w_idx],w_idx,0);
               ad5243_quick_cmd(0,INC_RDAC_6DB,w_idx);				  	    
               break;
           case UI_CMD_DEC:
               w_pos[w_idx]--;
               //ad5243_write(0,w_pos[w_idx],w_idx,0);
               ad5243_quick_cmd(0,DEC_RDAC_6DB,w_idx);				  	    
               break;
       }  
   ui_cmd = UI_CMD_NULL;     
 }
