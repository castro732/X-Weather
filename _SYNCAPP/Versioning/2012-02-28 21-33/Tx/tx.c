#if defined(__PCH__)
#include <tx.h>         // Headers
#endif
#include <bootloader.h> // Bootloader
  
  
  
/*VARIABLES**GLOBALES*/



/*FIN**VARIBLES**GLOBALES*/
  
  
  
  
  
/*DEFINICIONES*/  
#define LCD_RS_PIN      PIN_B4 
#define LCD_RW_PIN      PIN_B6   
#define LCD_ENABLE_PIN  PIN_B7    
#define LCD_DATA4       PIN_D4                                
#define LCD_DATA5       PIN_D5                                   
#define LCD_DATA6       PIN_D6                               
#define LCD_DATA7       PIN_D7
#define LCD_TYPE 2




#define INT_PIN   PIN_B0         //Interrupcion Externa - Velocidad Viento

/*FIN**DEFINICIONES*/  



/**INCLUDES**/

#include <LCD.C>        // LCD Driver
#include <math.h>       
#include <stdio.h>
#include "bmp085.c"     //BMP085 Driver  

/*FIN**INCLUDES*/

/*
#int_TIMER1
void  TIMER1_isr(void) 
{

}*/

#int_EXT
void  EXT_isr(void) 
{

}

#int_RDA
void  RDA_isr(void) 
{

}



void main()
{  
   signed int32 tbmp;
   int32 pbmp;
   int8 type;
   /*SETUP*/
   //setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);      //262 ms overflow
   setup_timer_3(T3_DISABLED|T3_DIV_BY_1);
   lcd_init();
      
  // enable_interrupts(INT_TIMER1);   //Overflow Interrupt
  // enable_interrupts(INT_EXT);      // External Interrupt
  // enable_interrupts(INT_RDA);      // Serial interrupt
  // enable_interrupts(GLOBAL);
   //setup_oscillator(OSC_8MHZ|OSC_INTRC|OSC_31250|OSC_PLL_OFF);
   /*END**SETUP*/
   
   
   BMP085_Calibration();   
   
   lcd_putc("\fX-Weather");
   printf(lcd_putc,"%LX",ac1);
   delay_ms(4000);
   While(1)
   {
   
      bmp085Convert(&tbmp,&pbmp);
      
      signed int16 temp;
      int32 pres;
      
      temp = tbmp;
      pres = pbmp;
      
      
      type = "T";
      printf("%c",type);
      printf("%3.1w\r",temp);
      type = "P";
      printf("%c",type);
      printf("%ld\r",pres);
      
      
      lcd_putc("\fX-Weather\n");       
      lcd_putc("Temp"); 
      lcd_gotoxy(6,2);
      printf(lcd_putc,"%3.1w",temp);
      lcd_putc("C");
      delay_ms(5000);
      
      lcd_putc("\fX-Weather\n");       
      lcd_putc("Pres"); 
      lcd_gotoxy(6,2);
      printf(lcd_putc,"%lu ",pres);
      lcd_putc("Pa");
      delay_ms(5000);
      
      
      
      
      //ENDWHILE
   }
   
   
   
   
   
   //ENDMAIN
}
