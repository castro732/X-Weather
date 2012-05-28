/************************************************************************
*                                                                       *
* Module:            tx.c                                               *
* Description:       Code for the X-Weather Transmitter.                *
*                                                                       *
*                                                                       *
* Date:            Authors:               Comments:                     *
* 30 Mar 2012      Jose Morales            Created                      *
*                                                                       *
*                                                                       *
************************************************************************/

#if defined(__PCH__)
#include <tx.h>         // Headers
#endif
//#include <bootloader.h> // Bootloader

  
/************************************************************************
*  Globar Vars and Constants ***** Variables y Constates globales       *
************************************************************************/
short dir0,dir1,dir2=0;
int dir=0;

int16 dhth,dhtt=0;
float32 tbmp,pbmp,abmp;

float timer,vel,aux;
int tim1,ext=0;
int32 aux_multp,start_send=0;
int16 pr_light,ls0,ls1,ls2=0;
int ls=0;
short go=1;
int weather;
short polled_vel,polled_bmp,polled_dht,polled_light,polled_dir=0;

int at;
int8 type[]="TPAHV";

                                 
  
/************************************************************************
*  Defines  *****  Definiciones                                         *
************************************************************************/

#define PIN_DIR0   PIN_D0
#define PIN_DIR1   PIN_D1
#define PIN_DIR2   PIN_D2






/************************************************************************
*  Includes  *****  Inclusiones                                         *
************************************************************************/
//#include <LCD.C>        // LCD Driver
#include <math.h>       
#include <stdio.h>
#include "../Libs/bmp085.c"      //BMP085 Driver  
#include "../Libs/dht22.c"       //DHT22 Driver  




/************************************************************************
*  Interrupts  *****  Interrupciones                                    *
************************************************************************/

#int_TIMER1
void  TIMER1_isr(void) 
{
   tim1++;
   
   set_timer1(0);
}

#int_TIMER3
void  TIMER3_isr(void) 
{
   start_send++;
   aux_multp++;
   set_timer3(0);
}

#int_EXT
void  EXT_isr(void) 
{
  ext = 1;
}

#int_RDA
void  RDA_isr(void) 
{

}



void poll_vel();
void poll_bmp();
void poll_dht();
void poll_light();
void poll_dir();

/************************************************************************
*  Main Program  *****  Programa Principal                              *
************************************************************************/
void main()
{  
   
   /*SETUP*/
   setup_adc_ports( AN0_TO_AN2 );
   setup_adc(ADC_CLOCK_DIV_64);

   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
   setup_timer_3(T3_INTERNAL|T3_DIV_BY_8);

   //enable_interrupts(INT_RDA);
   //enable_interrupts(INT_TIMER1); 

   enable_interrupts(INT_TIMER3); 
   enable_interrupts(GLOBAL);   
   
   /*END**SETUP*/
   
  
   
   /*Iniciamos el DHT22*/
   //iniciar_dht();
   
   /*Calibramos el BMP085*/
   //BMP085_Calibration();   
   


   /* Entramos en el Bucle Principal*/
   While(1)
   {
      

      while(start_send>20)
      {
         if(go)
         {
            aux_multp=0;
            go=0;
         }

         if((aux_multp>0)&&(polled_vel==0))           //Velocidad
         {
            poll_vel();
            polled_vel=1;
         }
         else if((aux_multp>25)&&(polled_bmp==0))     //BMP
         {
            //poll_bmp();
            char ttbmp[] = "263";
            printf("%c",type[0]);
            printf("%s\r",tbmp);

            char tpbmp[] = "8541";
            printf("%c",type[1]);
            printf("%s\r",pbmp);

            char tabmp[] = "15241";
            printf("%c",type[2]);
            printf("%s\r",abmp);

            polled_bmp=1;
         }
         else if((aux_multp>50)&&(polled_dht==0))     //DHT
         {
            //poll_dht();
            dhth = 654;
            printf("%c",type[3]);
            printf("%Ld\r",dhth);

            polled_dht=1;
         }  
         else if((aux_multp>75)&&(polled_light==0))   //Luz
         {
            poll_light();
            if((ls=8)||(ls=9))
            {
               weather = 1;               
            }
            else if((ls<8)&&(dhth<750))
            {
               weather = 2;
            }
            else if((ls<8)&&(dhth>750))
            {
               weather = 3;
            }

            printf("W%u\r",weather);

            polled_light=1;            
         }
         else if((aux_multp>100)&&(polled_dir==0))  //Direccion
         {
            poll_dir();

            polled_dir=1;
         }
         else if(aux_multp>125)
         {
            polled_vel=0;
            polled_bmp=0;
            polled_dht=0;
            polled_light=0;
            polled_dir=0;

            start_send=0;
            go=1;
         }

      }
      
      
      
      
      //ENDWHILE
   }
   
   
   
   
   
   //ENDMAIN
}


void poll_vel()
{
   
   enable_interrupts(INT_TIMER1);   
   enable_interrupts(INT_EXT_H2L);  
   set_timer1(0);

   while(1)
   {
      if(tim1>20)
      {
         vel = 0;
         printf("%c",type[4]);
         printf("%g\n",vel);
         tim1=0;
         ext=0;
         set_timer1(0);
      }
      if(ext==1)
      {
         timer = get_timer1();

         timer = timer * 0.000004;
         
         aux = tim1 * 0.065536;
         
         timer = timer + aux;
         vel = 0.0942/timer;
         vel = vel/0.5144;

         vel = vel*10;
         printf("%c",type[4]);
         printf("%Ld\r",vel);
    
      

         tim1=0;
         ext=0;
         set_timer1(0);
         break;

      }
      if(aux_multp>24)
      {
         break;
      }
   }
   
   disable_interrupts(INT_TIMER1);   
   disable_interrupts(INT_EXT_H2L);  
   
}

void poll_light()
{
   

   set_adc_channel(0);
   delay_us(10);
   ls0 = read_adc();
   delay_ms(10);
   set_adc_channel(1);
   delay_us(10);
   ls1 = read_adc();
   delay_ms(10);
   set_adc_channel(2);
   delay_us(10);
   ls2 = read_adc();

   pr_light=(ls0+ls1+ls2)/3; //Promedio
   if(pr_light<103)
   {
      ls=0;
   }
   else if (pr_light<206)
   {
      ls=1;  
   }
   else if (pr_light<309)
   {
      ls=2;
   }
   else if (pr_light<412)
   {
      ls=3;
   }
   else if (pr_light<515)
   {
      ls=4;
   }
   else if (pr_light<618)
   {
      ls=5;
   }
   else if (pr_light<721)
   {
      ls=6;
   }
   else if (pr_light<824)
   {
      ls=7;
   }
   else if (pr_light<927)
   {
      ls=8;
   }
   else if (pr_light>=927)   
   {
      ls=9;
   }
   printf("L%u\r",ls);

  
}

void poll_bmp()
{
   bmp085Convert(&tbmp,&pbmp);
   abmp = 44330*(1-(pow((pbmp/1013.25),(1/5.255))));

   tbmp = tbmp*10;
   printf("%c",type[0]);
   printf("%Ld\r",tbmp);

   pbmp = pbmp*10;
   printf("%c",type[1]);
   printf("%Ld\r",pbmp);

   abmp = abmp*10;
   printf("%c",type[2]);
   printf("%Ld\r",abmp);

}

void poll_dht()
{
   leer_dht(&dhth,&dhtt);
   printf("%c",type[3]);
   printf("%Ld\r",dhth);
}

void poll_dir()
{
   dir0 = input(PIN_DIR0);
   dir1 = input(PIN_DIR1);
   dir2 = input(PIN_DIR2);

   if(!dir2)
   {
      dir += 4;
   }
   if(!dir1)
   {
      dir += 2;
   }
   if(!dir0)
   {
      dir += 1;
   }

}
