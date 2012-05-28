#if defined(__PCH__)
#include <rx.h>         // Headers
#endif
#include <bootloader.h>
  

  
/*DEFINICIONES*/  


//#define GLCD_RST     PIN_C0
//#define GLCD_CS1     PIN_A5   // Chip Selection 1
#define FAST_GLCD




#define BUFFER_SIZE 32     //Buffer Serial
#define bkbhit (next_in!=next_out) //Serial

/*FIN**DEFINICIONES*/  



/*VARIABLES**GLOBALES*/

BYTE buffer[BUFFER_SIZE];  //Buffer Serial
BYTE next_in = 0;          //Buffer Serial
BYTE next_out = 0;         //Buffer Serial


int8 i;
int8 control = 0;
int pressed = 0;
int config=0;
int firstrun = 1;
int confpos,conftype,selconf;
int up,lastup;
int down,lastdown;
int enter,lastenter;
int left,lastleft;
int lastright,right;



char MENUTITLE[] = "Configuracion";

char MENU1[] =   "-Fecha y Hora";

char MENU2[] =   "-Temperatura   ";
char MENU2ITEM1[] = "--Celsius";
char MENU2ITEM2[] = "--Farenheit";

char MENU3[] =   "-Presion";
char MENU3ITEM1[] = "--Pascal";
char MENU3ITEM2[] = "--Milibar";


char MENU4[] =   "-Velocidad";
char MENU4ITEM1[] = "--m/s";
char MENU4ITEM2[] = "--km/h";
char MENU4ITEM3[] = "--Nudos";


char dia[] = "01";
char mes[] = "01";
char ao[] = "01";
char hora[] = "10";
char min[] = "10";
char aop[] = "AM";


char UniTemp[3] = "C";
char UniPres[4] = "Pa ";

char UniVel[5] = "m/s ";

/*FIN**VARIBLES**GLOBALES*/




/**INCLUDES**/

#include "../HDM64GS12.c"
#include "../graphics.c"



#include <internal_eeprom.c>
#include <stdio.h>
#include <stdlib.h>

#include "../config/menu.c"
#include "show.c"

/*FIN**INCLUDES*/

int try=0;
/*INTERRUPCIONES*/

#INT_RDA
void  RDA_isr(void) 
{
         int t; 
   
   try = 1;

   buffer[next_in]=getc(); 
   t=next_in; 
   if(++next_in==BUFFER_SIZE) 
      next_in=0; 
   if(next_in==next_out) 
      next_in=t;    // Buffer full !! 

  
}

#INT_EXT1
void EXT1_isr(void)
{
   config = 1;
   
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

void str_init(char *name)
{
   int8 z;
   for (z=0;z<15;z++)
      {
         name[z]=0;
         
      }
}

void main()
{

   
   
   enable_interrupts(INT_RDA);
   enable_interrupts(INT_EXT1);
   enable_interrupts(global);
   
   setup_psp(PSP_DISABLED);
   setup_wdt(WDT_OFF);
   setup_timer_0(RTCC_INTERNAL);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_timer_3(T3_DISABLED|T3_DIV_BY_1);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   
   setup_adc( ADC_OFF );


   //setup_adc_ports(NO_ANALOGS|VREF_VREF);


  // setup_oscillator(OSC_8MHZ|OSC_TIMER1|OSC_PLL_OFF);   
   
   
   glcd_init(ON);    
   
   
   
         int8 axt,axp,axv;
         axt = read_eeprom(0x01);
         axp = read_eeprom(0x02);
         axv = read_eeprom(0x03);
   
         if(axt==1)
         {
            UniTemp[0]="C";
         }
         if(axt==2)
         {
            UniTemp[0]="F";
         }
         if(axp==1)
         {
            UniPres[0]="Pa";
         }
         if(axp==2)
         {
            UniPres[0]="mBa";
         }
         if(axv==1)
         {
            UniVel[0]="m/s";
         }
         if(axv==2)
         {
            UniVel[0]="km/h";
         }
         if(axv==3)
         {
            UniVel[0]="Nud";
         }
         
         
         
   
  
   
   int16 auxi;
   int8 type,z;
   int8 string[10];
   int16 tt;
   int32 pp;
   char tprint [15];
   char pprint [15];
   char ttt[10];
   char ppp[10];
      char temp[] = "T:";
      char pres[] = "P:";
      
   for (z=0;z<15;z++)
      {
         tprint[z]=0;
         pprint[z]=0;
      }
   //firstrun = read_eeprom(0x00);      
  

      draw_sect();
   while (1)
   {
     
      
      
      
      if (firstrun==71)
      {
         glcd_loadscreen();
         glcd_showlogo();
         delay_ms(1500);
         menu_mainmenu();
         firstrun = 0;
         write_eeprom(0x00, firstrun);
      }
      
      if (config==1)
      {
        
        
        
        
        
         menu_mainmenu();
         if(UniTemp[1]=="C")
         {
            write_eeprom(0x01, 1);
         }
         else if(UniTemp[1]=="F")
         {
            write_eeprom(0x01, 2);
         }
         if(UniPres[1]=="P")
         {
            write_eeprom(0x02, 1);
         }
         else if(UniPres[1]=="m")
         {
            write_eeprom(0x02, 2);
         }
         if(UniVel[1]=="m")
         {
            write_eeprom(0x03, 1);
         }
         else if(UniVel[1]=="k")
         {
            write_eeprom(0x03, 2);
         }
         else if(UniVel[1]=="N")
         {
            write_eeprom(0x03, 3);
         }
         
        
         
         pressed=0;
         config=0;
      }
      
      if (try==1)
      {
         
               try=0;
               output_toggle(PIN_C2);
      }
      
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
         if (auxi!=0x0D)
         {
            
         }
         if (type==1)      //Temperatura
         {
         
            if ((auxi!=0x0D)&&(auxi!=0x54))
            {
               string[i] = auxi;
               i++;
            }
            if (auxi==0x0D)
            {
               str_init(tprint);
               tt = atol(string);  
               sprintf(ttt,"%3.1w",tt);
               strcat(tprint,temp);
               strcat(tprint,ttt);
               strcat(tprint,UniTemp);
                draw_sect();
               glcd_text_sec(1, 1, tprint, OFF);
               glcd_update();
               
            }
         }
         if (type==2)      //Presion
         {
            if ((auxi!=0x0D)&&(auxi!=0x50))
            {
               string[i] = auxi;
               i++;
            }
            if (auxi==0x0D)
            {
               str_init(pprint);
               pp = atoi32(string);
               sprintf(ppp,"%3.1w",pp);
               strcat(pprint,pres);
               strcat(pprint,ppp);
               strcat(pprint,UniPres);  
                draw_sect();
               glcd_text_sec(1, 2, pprint, OFF);
               glcd_update();
            }
         }
         
         glcd_update();
         //END KBHIT
      }
      
      
      glcd_update();
      //ENDWHILE
   }
//ENDMAIN
}
