#if defined(__PCH__)
#include <rx.h>         // Headers
#endif
#include <bootloader.h> // Bootloader
  
  
  
/*DEFINICIONES*/  
#define LCD_RS_PIN      PIN_B4 
#define LCD_RW_PIN      PIN_B6   
#define LCD_ENABLE_PIN  PIN_B7    
#define LCD_DATA4       PIN_D4                                
#define LCD_DATA5       PIN_D5                                   
#define LCD_DATA6       PIN_D6                               
#define LCD_DATA7       PIN_D7
#define LCD_TYPE 2




#define BUFFER_SIZE 32     //Buffer Serial
#define bkbhit (next_in!=next_out) //Serial

/*FIN**DEFINICIONES*/  



/*VARIABLES**GLOBALES*/

BYTE buffer[BUFFER_SIZE];  //Buffer Serial
BYTE next_in = 0;          //Buffer Serial
BYTE next_out = 0;         //Buffer Serial

/*FIN**VARIBLES**GLOBALES*/





/**INCLUDES**/

#include <LCD.C>        // LCD Driver      
#include <stdio.h>
#include <stdlib.h>

/*FIN**INCLUDES*/


/*INTERRUPCIONES*/

#INT_RDA
void  RDA_isr(void) 
{
         int t; 

   buffer[next_in]=getc(); 
   t=next_in; 
   if(++next_in==BUFFER_SIZE) 
      next_in=0; 
   if(next_in==next_out) 
      next_in=t;    // Buffer full !! 

  
}

/*FIN**INTERRUPCIONES*/


/*FUNCIONES*/

int8 bgetc() { 
   int8 c; 

   while(!bkbhit) ; 
   c=buffer[next_out]; 
   next_out=(next_out+1) % BUFFER_SIZE; 
   
   return(c); 
} 

/*FIN**FUNCIONES*/


void main()
{

   lcd_init();
   
   enable_interrupts(int_rda);
   enable_interrupts(global);
   
   setup_psp(PSP_DISABLED);
   setup_wdt(WDT_OFF);
   setup_timer_0(RTCC_INTERNAL);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_timer_3(T3_DISABLED|T3_DIV_BY_1);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   
   setup_adc_ports(NO_ANALOGS|VREF_VREF);


   setup_oscillator(OSC_8MHZ|OSC_INTRC|OSC_31250|OSC_PLL_OFF);

   while(TRUE)
   {
         int8 type;
         int16 auxi;
         int16 tt,hh=0;
         int32 hhh=0;
         char string[15];
         int8 i=0;
         while (1)
         {
                  for(i=0;i<15;i++)
                  {                   
                  string[i]=0x00;                          
                  }
              i = 0;
               while(bkbhit) 
               
               {    
                  
                                
                  
                  
                       
                       
                        auxi = bgetc();
                        if (auxi==0x54)
                        {
                           type=1;
                           for(i=0;i<15;i++)
                           {                   
                           string[i]=0x00;                          
                           }
                           i = 0;
                        }
                        if (auxi==0x50)
                        {
                           type=2;
                           for(i=0;i<15;i++)
                           {                   
                           string[i]=0x00;                          
                           }
                           i = 0;
                        }
                        if (type==1)
                        {
                           
                           if ((auxi!=0x0D)&&(auxi!=0x54))
                           {
                              string[i] = auxi;
                              i++;
                              
                              
                           }
                           if (auxi==0x0D)
                           {
                              hh = atol(string);
                              lcd_putc("\fTemp "); 
                              printf(lcd_putc,"%ld",hh);
                              lcd_putc("C");
                           }
                        }
                        if (type==2)
                        {
                           if ((auxi!=0x0D)&&(auxi!=0x50))
                           {
                              string[i] = auxi;
                              i++;
                              
                              
                           }
                           if (auxi==0x0D)
                           {
                              hhh = atoi32(string);
                              lcd_putc("\fPresion "); 
                              printf(lcd_putc,"%ld ",hhh);
                              lcd_putc("Pa");
                           }
                        }
               }
      
      
      
      
      
         }
      
   //ENDWHILE      
   }
//ENDMAIN
}
