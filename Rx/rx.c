#if defined(__PCH__)
#include <rx.h>         // Headers
#endif
//#include <bootloader.h>
  

  
/************************************************************************
*  Defines  *****  Definiciones                                         *
************************************************************************/

#define FAST_GLCD

#define BUFFER_SIZE 64     //Buffer Serial
#define bkbhit (next_in!=next_out) //Serial

#define LED_WARNING PIN_C2



/************************************************************************
*  Globar Vars and Constants ***** Variables y Constates globales       *
************************************************************************/

int buffer[BUFFER_SIZE];  //Buffer Serial
int next_in = 0;          //Buffer Serial
int next_out = 0;         //Buffer Serial

int temp_buffer_eeprom[128];
int pres_buffer_eeprom[128];
int humi_buffer_eeprom[128];
int alt_buffer_eeprom[128];
int vel_buffer_eeprom[128];


int8 i;
int16 y,eep=0;

int config=0;
int firstrun = 1;

int8 tim1=0;
int16 timer_ok=0;

int16 auxi;
int8 type,z;
int8 string[10];

char tprint [15];
char t2_print[15];
char pprint [15];
char hprint [15];
char aprint [15];
char pdlprint [15];
char dprint [15];
char vprint [15];

int16 tt,hh,ppdl,dd,vv,ww,okst,bat1_st,bat2_st;
int32 pp,aa;

char ttt[10];
char ppp[10];
char hhh[10];
char aaa[10];
char pppdl[10];
char ddd[10];
char vvv[10];
   
int ll=67;
int8 ok[]="OK!";
int rcv_t,rcv_td,rcv_p,rcv_h,rcv_a,rcv_pdl,rcv_d,rcv_l,rcv_v,rcv_w,rcv_ok,rcv_bat1_st=0;

/*Vars for date and time*/
int16 uh=0;
int aux_hr,aux_min,aux_sec,aux_day,aux_month,aux_year,aux_dow;
char print_time[10];
char print_date[20];
char print_day[10];
char print_month[10];
char print_year[10];
char print_dow[10];
char print_hr[10];
char print_min[10];

int twop[3];
int guion[3];
int spa[3];

/************************************************************************
*  Includes  *****  Inclusiones                                         *
************************************************************************/

#include "../Libs/HDM64GS12.c"
//#include "../Libs/Mikro-HDM64GS12.c"
#include "../Libs/graphics.c"

#include "../Libs/ds1307.c"

#include <internal_eeprom.c>
#include <stdio.h>
#include <stdlib.h>

//#include "../Libs/menu2.c"
#include "../config/menu.c"
#include "show.c"

#include "24FC1025.c"





int try=0;


/************************************************************************
*  Interrupts  *****  Interrupciones                                    *
************************************************************************/

#INT_RDA
void  RDA_isr(void) 
{
   int t; 
   
   timer_ok=0;

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
   output_high(PIN_LED);
}

#INT_TIMER1
void TIM1_isr(void)
{
   tim1 += 1;
   uh++;
   if(tim1>25)
   {
      output_low(PIN_LED);
      tim1 = 0;
      set_timer1(0);
   }

}
#INT_TIMER2
void TIM2_isr(void)
{
   timer_ok++;
}



/************************************************************************
*  Function Prototypes                                                  *
************************************************************************/

int8 bgetc();
void str_init(char *name);
void readconfig(void);
void update_readings(void);
void lightbuttons(void);
void poll_ok(void);
void update_eeprom(void);

/************************************************************************
*  Main Program  *****  Programa Principal                              *
************************************************************************/
void main()
{

   setup_psp(PSP_DISABLED);
   setup_wdt(WDT_OFF);
   setup_timer_0(RTCC_INTERNAL);
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
   setup_timer_2(T2_DIV_BY_16,0xFF,16);
   setup_timer_3(T3_DISABLED|T3_DIV_BY_1);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   
   setup_adc( ADC_OFF );

   enable_interrupts(INT_TIMER1);
   enable_interrupts(INT_TIMER2);
   enable_interrupts(INT_RDA);
   enable_interrupts(INT_EXT1);
   enable_interrupts(global);
   

   str_init(print_date);
   str_init(print_time);
   
   
   
   
   
   set_timer1(0);
   
   init_ext_eeprom();
   ds1307_init();
   glcd_init(ON);    

   

   readconfig();

      
  

   draw_sect();

   while (1)
   { 
      
      
      
      if (firstrun==255)
      {
         glcd_fillscreen(OFF);
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
         if(UniTemp[0]==0x43)
         {
            write_eeprom(0x01, 1);
         }
         else if(UniTemp[0]==0x46)
         {
            write_eeprom(0x01, 2);
         }
         if(UniPres[0]==0x6D)
         {
            write_eeprom(0x02, 1);
         }
         else if(UniPres[0]==0x50)
         {
            write_eeprom(0x02, 2);
         }
         if(UniVel[0]==0x4E)
         {
            write_eeprom(0x03, 1);
         }
         else if(UniVel[0]==0x6B)
         {
            write_eeprom(0x03, 2);
         }
         else if(UniVel[0]==0x6D)
         {
            write_eeprom(0x03, 3);
         }
         
         update_readings();
         
         button = !PRESSED;
         config=0;
      }
      
      if (uh>40)
      {
       
         twop[0]=":";
         guion[0]="-";
         spa[0]=" ";  

         str_init(print_time);
         str_init(print_date);
         
         ds1307_get_time(aux_hr,aux_min,aux_sec);
         ds1307_get_date(aux_day, aux_month, aux_year, aux_dow);

         itoa(aux_day,10,print_day);
         itoa(aux_month,10,print_month);
         itoa(aux_year,10,print_year);
         switch(aux_dow)
         {
            case 0:
            {
               print_dow[0] = "D";
               break;
            }
            case 1:
            {
               print_dow[0] = "L";
               break;
            }
            case 2:
            {
               print_dow[0] = "M";
               break;
            }
            case 3:
            {
               print_dow[0] = "I";
               break;
            }
            case 4:
            {
               print_dow[0] = "J";
               break;
            }
            case 5:
            {
               print_dow[0] = "V";
               break;
            }
            case 6:
            {
               print_dow[0] = "S";
               break;
            }
         }
         sprintf(print_date,"%s:%s-%s-%s",print_dow,print_day,print_month,print_year);
         //strcat(print_date,print_dow);
         //strcat(print_date,twop);
         //strcat(print_date,print_day);
         //strcat(print_date,guion);
        // strcat(print_date,print_month);
         //strcat(print_date,guion);
         //strcat(print_date,print_year);

         if(aop[0]==0x50)
         {
            aux_hr=aux_hr-20;
         }
         itoa(aux_hr,10,print_hr);
         itoa(aux_min,10,print_min);
         strcat(print_time,print_hr);
         strcat(print_time,twop);
         strcat(print_time,print_min);
         
         if ((aop[0]==0x41)||(aop[0]==0x50))
         {
            strcat(print_time,twop);
            strcat(print_time,aop);
         }
         


         
         rcv_td=1;
         uh=0;
      }

      poll_ok();
      lightbuttons();

      while(bkbhit) 
      {
         
         auxi = bgetc();
         switch (auxi)
         {
            case 0x54:     //T
            {
               type=1;
               str_init(string);
               break;
            }
            case 0x50:     //P
            {
               type=2;
               str_init(string);
               break;
            }
            case 0x48:     //H
            {
               type=3;
               str_init(string);
               break;
            }
            case 0x41:     //A
            {
               type=4;
               str_init(string);
               break;
            }
            case 0x57:     //W
            {
               type=5;
               str_init(string);
               break;
            }
            case 0x52:     //R 
            {
               type=6;
               str_init(string);
               break;
            }
            case 0x44:     //D
            {
               type=7;
               str_init(string);
               break;
            }
            case 0x4C:     //L
            {
               type=8;
               str_init(string);
               break;
            }
            case 0x56:     //V
            {
               type=9;
               str_init(string);
               break;
            }
            case 0x42:     //B
            {
               type=10;
               str_init(string);
               break;
            }         
            
         }
         
         switch (type)
         {
            case 1:                 //Temperatura
            {
               if ((auxi!=0x0D)&&(auxi!=0x54))
               {
                  string[i] = auxi;
                  i++;
               }
               if (auxi==0x0D)
               {
                  i=0;
                  str_init(tprint);
                  tt = atol(string);  

                  if(UniTemp[0]==0x46)
                  {
                     tt = ((tt*9)/5)+320;
                  }

                  strcat(temp_buffer_eeprom,string);
                  eep+=3;

                  sprintf(ttt,"%3.1w",tt);
                  strcat(tprint,ttt);
                  strcat(tprint,UniTemp);

                  strcpy(t2_print,tprint);

                  rcv_t=1;
               }
               break;
            }
            case 2:                 //Presion
            {
               if ((auxi!=0x0D)&&(auxi!=0x50))
               {
                  string[i] = auxi;
                  i++;
               }
               if (auxi==0x0D)
               {
                  i=0;
                  str_init(pprint);
                  pp = atoi32(string);

                  if(UniPres[0]==0x50)
                  {
                     pp = pp*10;
                     sprintf(ppp,"%6.0w",pp);
                     strcat(pprint,ppp);
                     strcat(pprint,UniPres);  
                  }
                  else
                  {
                     sprintf(ppp,"%4.1w",pp);
                     strcat(pprint,ppp);
                     strcat(pprint,UniPres);  
                  }

                  rcv_p=1;
                  
               }
               break;
            }
            case 3:                 //Humedad
            {
               if ((auxi!=0x0D)&&(auxi!=0x48))
               {
                  string[i] = auxi;
                  i++;
               }
               if (auxi==0x0D)
               {
                  i=0;
                  char uni[]="%";
                  str_init(hprint);
                  hh = atol(string);
                  sprintf(hhh,"%2.1w",hh);
                  strcat(hprint,hhh);
                  strcat(hprint,uni);

                  rcv_h=1;
               }
               break;
            }
            case 4:                 //Altitud
            {
               if ((auxi!=0x0D)&&(auxi!=0x41))
               {
                  string[i] = auxi;
                  i++;
               }
               if (auxi==0x0D)
               {
                  i=0;
                  char uni[]="m";
                  str_init(aprint);
                  aa = atol(string);
                  sprintf(aaa,"%5.1w",aa);
                  strcat(aprint,aaa);
                  strcat(aprint,uni);

                  rcv_a=1;
                    
               }
               break;
            }
            case 5:                 //Clima
            {
               if ((auxi!=0x0D)&&(auxi!=0x57))
               {
                  string[i] = auxi;
                  i++;
               }
               if (auxi==0x0D)
               {
                  i=0;                  
                  
                  ww = atoi(string);
                                    
                  rcv_w=1;
               }
               break;
            }
            case 6:                 //Probabilidad de lluvia
            {
               if ((auxi!=0x0D)&&(auxi!=0x52))
               {
                  string[i] = auxi;
                  i++;
               }
               if (auxi==0x0D)
               {
                  i=0;

                  char uni[]="%";
                  str_init(pdlprint);
                  ppdl = atol(string);
                  sprintf(pppdl,"%2.0w",ppdl);
                  strcat(pdlprint,pppdl);
                  strcat(pdlprint,uni);
                  
                  rcv_pdl=1;
               }
               break;
            }
            case 7:                 //Direccion del viento
            {
               if ((auxi!=0x0D)&&(auxi!=0x44))
               {
                  string[i] = auxi;
                  i++;
               }
               if (auxi==0x0D)
               {
                  i=0;
                  
                  str_init(dprint);
                  dd = atol(string);
                  switch(dd)
                  {
                     case 0:
                     {
                        dprint[0]="N";
                        dprint[1]="\0";
                        break;
                     }
                     case 1:
                     {
                        dprint[0]="N";
                        dprint[1]="E";
                        dprint[2]="\0";
                        break;
                     }
                     case 2:
                     {
                        dprint[0]="E";
                        dprint[1]="\0";
                        break;
                     }
                     case 3:
                     {
                        dprint[0]="S";
                        dprint[1]="E";
                        dprint[2]="\0";
                        break;
                     }
                     case 4:
                     {
                        dprint[0]="S";
                        dprint[1]="\0";
                        break;
                     }
                     case 5:
                     {
                        dprint[0]="S";
                        dprint[1]="O";
                        dprint[2]="\0";
                        break;
                     }
                     case 6:
                     {
                        dprint[0]="O";
                        dprint[1]="\0";
                        break;
                     }
                     case 7:
                     {
                        dprint[0]="N";
                        dprint[1]="O";
                        dprint[2]="\0";
                        break;
                     }
                  }
                  
                  rcv_d=1;           
               }
               break;
            }
            case 8:                 //Intensidad de luz
            {
               if ((auxi!=0x0D)&&(auxi!=0x4C))
               {
                  string[i] = auxi;
                  i++;
               }
               if (auxi==0x0D)
               {
                  i=0;                  
                  
                  ll = atoi(string);
                  ll = ll*6.3+67;
                  
                  rcv_l=1;
               }
               break;
            }
            case 9:                 //Velocidad del viento
            {
               if ((auxi!=0x0D)&&(auxi!=0x56))
               {
                  string[i] = auxi;
                  i++;
               }
               if (auxi==0x0D)
               {
                  i=0;
                  
                  str_init(vprint);
                  vv = atoi32(string);

                  if(UniVel[0]==0x6D)
                  {
                     vv = vv * 0.5144;
                  }
                  else if(UniVel[0]==0x6B)
                  {
                     vv = vv * 1.852;
                  }
                  else
                  {

                  }

                  sprintf(vvv,"%4.1w",vv);
                  strcat(vprint,vvv);
                  strcat(vprint,UniVel);  

                  
                  rcv_v=1;
               }
               break;
            }
            case 10:                //Bat Status
            {
               if ((auxi!=0x0D)&&(auxi!=0x42))
               {
                  string[i] = auxi;
                  i++;
               }
               if (auxi==0x0D)
               {
                  i=0;
                  
                  
                  bat1_st = atoi(string);
                  
                  rcv_bat1_st=1;
               }
               break;
            }
            
         }

         if(eep>=9)
         {
            update_eeprom();
            eep=0;
         }
         update_readings();
         glcd_update();
         //END KBHIT 
      }
      

      
      glcd_update();
      //ENDWHILE
   }
//ENDMAIN
}


/************************************************************************
*                                                                       *
*  Purpose:    Get the char from the buffer                             *
*                                                                       *
*  Proposito:  Obtener el char del buffer                               *
*                                                                       *
*  Passed:     None                                                     *
*  Argumento:  Nada                                                     *
*  Returned:   Char from the buffer                                     *
*  Retorno:    Char del buffer                                          *
*  Note:                                                                *
*                                                                       *
*                                                                       *
************************************************************************/
int8 bgetc() 
{ 
   int8 c; 

   while(!bkbhit) ; 
   c=buffer[next_out]; 
   next_out=(next_out+1) % BUFFER_SIZE; 
   
   return(c); 
} 

/************************************************************************
*                                                                       *
*  Purpose:    Initiate an array to 0                                   *
*                                                                       *
*  Proposito:  Inicia un array a 0                                      *
*                                                                       *
*  Passed:     char *name - name of the array                           *
*  Argumento:  char *name - Nombre del array                            *
*  Returned:   None                                                     *
*  Retorno:    Nada                                                     *
*  Note:                                                                *
*                                                                       *
*                                                                       *
************************************************************************/
void str_init(char *name)
{
   int8 z;
   for (z=0;z<15;z++)
      {
         name[z]=0;
         
      }
}

/************************************************************************
*                                                                       *
*  Purpose:    *
*                                                                       *
*  Proposito:  *
*                                                                       *
*  Passed:     *
*  Argumento:  *
*  Returned:   *
*  Retorno:    *
*  Note:       *
*                                                                       *
*                                                                       *
************************************************************************/
void readconfig(void)
{
   firstrun = read_eeprom(0x00);

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
      UniPres[0]="m";
      UniPres[1]="B";
      UniPres[2]="a";
   }
   if(axp==2)
   {
      UniPres[0]="P";
      UniPres[1]="a";
      UniPres[2]=" ";
   }
   if(axv==1)
   {
      UniVel[0]="N";
      UniVel[1]="u";
      UniVel[2]="d";
      UniVel[3]=" ";
   }
   if(axv==2)
   {
      UniVel[0]="k";
      UniVel[1]="m";
      UniVel[2]="/";
      UniVel[3]="h";
   }
   if(axv==3)
   {
      UniVel[0]="m";
      UniVel[1]="/";
      UniVel[2]="s";
      UniVel[3]=" ";
   }
}

/************************************************************************
*                                                                       *
*  Purpose:    *
*                                                                       *
*  Proposito:  *
*                                                                       *
*  Passed:     *
*  Argumento:  *
*  Returned:   *
*  Retorno:    *
*  Note:       *
*                                                                       *
*                                                                       *
************************************************************************/
void update_eeprom(void)
{  
   write_ext_eeprom_page(0,0,128,temp_buffer_eeprom);
}
void update_readings(void)
{
   draw_sect();
   
   //Sec1
   if(rcv_t==1)
   {
      glcd_text_sec(1, 1, t2_print, OFF);
   }
   if(rcv_p==1)
   {
      glcd_text_sec(1, 2, pprint, OFF);
   }
   if(rcv_h==1)
   {
      glcd_text_sec(1, 3, hprint, OFF);
   }

   //Sec2
   if(rcv_a==1)
   {
      glcd_text_sec(2, 1, aprint, ON);
   }
   if(rcv_v==1)
   {
      glcd_text_sec(2, 2, vprint, ON);
   }
   if(rcv_l==1)
   {
      glcd_rect(65, 21, 125, 28, NO, ON);
      glcd_rect(67, 23, ll, 26, YES, ON);
   }

   //Sec3
   if(rcv_w==1)
   {
      switch(ww)
      {
         case 1:
         {
            glcd_showimg_xy(sun,3, 32, NO);
            break;
         }
         case 2:
         {
            glcd_showimg_xy(cloud,3, 32, NO);
            break;
         }
         case 3:
         {
            glcd_showimg_xy(rain,3, 32, NO);
            break;   
         }
         case 4:
         {
            glcd_showimg_xy(snow,3, 32, NO);
            break;
         }
      }
      
   }
   if(rcv_pdl==1)
   {
      glcd_text_sec(3, 1, pdlprint, ON);
   }
   if(rcv_d==1)
   {
      glcd_text_sec(3, 2, dprint, ON);
   }

   //Sec4
   if(rcv_bat1_st==1)
   {
      switch(bat1_st)
      {
         case 0:
         {
            glcd_showimg_xy(nobat,66, 33, YES);
            break;
         }
         case 1:
         {
            glcd_showimg_xy(bat1,66, 33, YES);
            break;
         }
         case 2:
         {
            glcd_showimg_xy(bat2,66, 33, YES);
            break;
         }
         case 3:
         {
            glcd_showimg_xy(bat3,66, 33, YES);
            break;
         }
         case 4:
         {
            glcd_showimg_xy(fullbat,66, 33, YES);
            break;
         }
         
      }

       
   }

   switch(bat2_st)
   {
      case 0:
      {
         glcd_showimg_xy(nobat,88, 33, YES);
         break;
      }
      case 1:
      {
         glcd_showimg_xy(bat1,88, 33, YES);
         break;
      }
      case 2:
      {
         glcd_showimg_xy(bat2,88, 33, YES);
         break;
      }
      case 3:
      {
         glcd_showimg_xy(bat3,88, 33, YES);
         break;
      }
      case 4:
      {
         glcd_showimg_xy(fullbat,88, 33, YES);
         break;
      }
      
   }
   
      switch(okst)
      {
         case 0:
         {  
            glcd_showimg_xy(warning,115, 33, YES);
            break;
         }
         case 1:
         {
            
            glcd_text57(110,33,ok,1,OFF);
            break;
         }
      }
   
   if(rcv_td==1)
   {
   glcd_text_sec(4,2,print_date,OFF);
   glcd_text_sec(4,3,print_time,OFF);
   }    
   
}

/************************************************************************
*                                                                       *
*  Purpose:    Read the state of the buttons to turn on the GLCD led    *
*                                                                       *
*  Proposito:  Lee el estado de los botones para encender la luz GLCD   *
*                                                                       *
*  Passed:     None                                                     *
*  Argumento:  Nada                                                     *
*  Returned:   None                                                     *
*  Retorno:    Nada                                                     *
*  Note:                                                                *
*                                                                       *
*                                                                       *
************************************************************************/
void lightbuttons(void)
{   
   up = input(PIN_UP);
   down = input(PIN_DOWN);
   left = input(PIN_LEFT);
   right = input(PIN_RIGHT);
   enter = input(PIN_ENTER);
   if ((up==PRESSED)||(down==PRESSED)||(enter==PRESSED)||(left==PRESSED)||(right==PRESSED))
   {      
      up=!PRESSED;
      down=!PRESSED;
      left=!PRESSED;
      right=!PRESSED;
      enter=!PRESSED;
      tim1 = 0;   
      output_high(PIN_LED);
      set_timer1(0);
   }        
       
}


/************************************************************************
*                                                                       *
*  Purpose:    *
*                                                                       *
*  Proposito:  *
*                                                                       *
*  Passed:     *
*  Argumento:  *
*  Returned:   *
*  Retorno:    *
*  Note:       *
*                                                                       *
*                                                                       *
************************************************************************/

void poll_ok(void)
{  
   if(timer_ok>1520)  //1955 for 5min
   {
      okst=0;
      output_high(LED_WARNING);
      update_readings();
      
   }
   else
   {
      okst=1;
      output_low(LED_WARNING);
      update_readings();
      
   }
   
}