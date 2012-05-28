//#include "config.h"


/*DEFINICIONES*/  
#ifndef FAST_GLCD
#define FAST_GLCD
#endif

#ifndef PIN_UP 
#DEFINE PIN_UP       PIN_A0
#endif

#ifndef PIN_LEFT 
#DEFINE PIN_LEFT     PIN_A1
#endif

#ifndef PIN_RIGHT
#DEFINE PIN_RIGHT    PIN_A2
#endif

#ifndef PIN_DOWN
#DEFINE PIN_DOWN     PIN_A3
#endif

#ifndef PIN_ENTER
#DEFINE PIN_ENTER    PIN_A4
#endif

#ifndef PIN_CANCEL
#DEFINE PIN_CANCEL   PIN_A5
#endif





/*FIN**DEFINICIONES*/  


/****/


/*VARIABLES**GLOBALES*/

int i = 0;



int8 control = 0;

int pressed = 0;
int config;
int confpos,conftype,selconf;
int up,lastup;
int down,lastdown;
int enter,lastenter;
int left,lastleft;
int lastright,right;

/*MENU*/

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


char UniTemp[] = "C";
char UniPres[] = "Pa ";

char UniVel[] = "m/s ";


/*FIN**VARIBLES**GLOBALES*/



/****/


/**INCLUDES**/

#include "HDM64GS12.c"
#include "graphics.c"
#include <stdlib.h>
#include <string.h>

#include "../config/menu.c"

/*FIN**INCLUDES*/


/****/


/*FUNCIONES*/


/*FIN**FUNCIONES*/


void main()
{

   
   glcd_init(ON);  

   delay_ms(100);
   
   
    
   
   
   menu_mainmenu();
   
  
   
}


/*FUNCIONES*/


/*FIN**FUNCIONES*/
