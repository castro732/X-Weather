/************************************************************************
*                                Menu.c                                 *
*                                                                       *
*     Ester archivo contiene las funciones para implementar un menu     *
*     en una pantalla lcd grafica de 128x64 con controlador KS0108      *
************************************************************************/

/************************************************************************
*                                                                       *
*        readbuttons(void)                                              *
*           * Lee los estados de los botones.                           *
*                                                                       *
*                                                                       *
*        menu_entrar(int8 tb, int8 color)                               *
*           * Dibuja una flecha al final de una linea de texto.         *
*              - tb es el pixel donde comienza el texto.                *
*              - color puede ser ON o OFF                               *
*                                                                       *
*        menu_select(char item, char menu)                              *
*           * Selecciona una linea de texto invirtiendo los colores.    *
*              - item es la linea a seleccionar.                        *
*              - menu identifica el menu principal o submenu            *
*                                                                       *
*        menu_submenu(char menu)                                        *
*           * Selecciona el submenu a dibujar                           *
*              - menu es el numero de submenu                           *
*                                                                       *
*        menu_mainmenu(void)                                            *
*           * Dibuja el menu principal y maneja los submenu             *
*                                                                       *
*        menu_timemenu(void)                                            *
*           * Dibuja y maneja el setup de fecha y hora                  *
*                                                                       *
************************************************************************/


/************************************************************************
*  Defines  *****  Definiciones                                         *
************************************************************************/
#ifndef PRESSED 
#define PRESSED 1
#endif

#ifndef PIN_UP 
#define PIN_UP       PIN_A0
#endif

#ifndef PIN_LEFT 
#define PIN_LEFT     PIN_A1
#endif

#ifndef PIN_RIGHT
#define PIN_RIGHT    PIN_A2
#endif

#ifndef PIN_DOWN
#define PIN_DOWN     PIN_A3
#endif

#ifndef PIN_ENTER
#define PIN_ENTER    PIN_A4
#endif

#ifndef PIN_LED
#define PIN_LED    PIN_C1
#endif

#ifndef MENUT

char MENUTITLE[] = "Configuracion";

/*Menu 1*/
char MENU1[] =   "-Fecha y Hora";

/*Menu 2*/
char MENU2[] =   "-Temperatura   ";
char MENU2ITEM1[] = "--Celsius";
char MENU2ITEM2[] = "--Farenheit";

/*Menu 3*/
char MENU3[] =   "-Presion";
char MENU3ITEM1[] = "--Milibar";
char MENU3ITEM2[] = "--Pascal";

/*Menu 4*/
char MENU4[] =   "-Velocidad";
char MENU4ITEM1[] = "--Nudos";
char MENU4ITEM2[] = "--km/h";
char MENU4ITEM3[] = "--m/s";

/*Hora y fecha*/
int k = 0;

char dow[] = "L";
char dia[] = "01";
char mes[] = "01";
char ao[] = "01";
char hora[] = "10";
char min[] = "10";
char aop[] = "AM";

/*Unidades*/
char UniTemp[3] = "C";
char UniPres[4] = "mBa";
char UniVel[5] = "Nud ";


#endif






/************************************************************************
*  Globar Vars and Constants ***** Variables y Constates globales       *
************************************************************************/
int button = 0;
int8 control = 0;
int up,lastup;
int down,lastdown;
int enter,lastenter;
int left,lastleft;
int lastright,right;


/************************************************************************
*  Function Prototypes                                                  *
************************************************************************/
void readbuttons(void);
void menu_entrar(int8 tb, int8 color);
void menu_select(char item, char menu);
void menu_submenu(char menu);
void menu_mainmenu(void);
void menu_timemenu(void);
void menu_drawmain(void);


/************************************************************************
*                                                                       *
*  Purpose:    Read the state of the buttons                            *
*                                                                       *
*  Proposito:  Lee el estado de los botones                             *
*                                                                       *
*  Passed:     None                                                     *
*  Argumento:  Nada                                                     *
*  Returned:   None                                                     *
*  Retorno:    Nada                                                     *
*  Note:                                                                *
*                                                                       *
*                                                                       *
************************************************************************/
void readbuttons(void)
{
   while (button==!PRESSED)
   {
      lastup = up;
      lastdown = down;
      lastleft = left;
      lastright = right;
      lastenter = enter;
      up = input(PIN_UP);
      down = input(PIN_DOWN);
      left = input(PIN_LEFT);
      right = input(PIN_RIGHT);
      enter = input(PIN_ENTER);
      if ((up!=lastup)||(down!=lastdown)||(enter!=lastenter)||(lastleft!=left)||(lastright!=right))
      {
         button = PRESSED;
         set_timer1(0);
         tim1 = 0;   
         output_high(PIN_LED);
      }        
   }  
}

/************************************************************************
*                                                                       *
*  Purpose:    Draw the arrow indicating the existence of a submenu     *
*                                                                       *
*  Proposito:  Dibuja la flecha que indica que hay un submenu           *
*                                                                       *
*  Passed:     int8 tb - Y position of the arrow                        *
*              int8 color - Color of the arrow.                         *
*  Argumento:  int8 tb - Posicion en Y de la flecha                     *
*              int8 color - Color de la flecha.                         *
*  Returned:   None                                                     *
*  Retorno:    Nada                                                     *
*  Note:                                                                *
*                                                                       *
*                                                                       *
************************************************************************/
void menu_entrar(int8 tb, int8 color)
{
   glcd_line(124,tb,124,tb+7,color);
   glcd_line(125,tb+1,125,tb+6,color);
   glcd_line(126,tb+2,126,tb+5,color);
   glcd_line(127,tb+3,127,tb+4,color);
   
}

/************************************************************************
*                                                                       *
*  Purpose:    Draw the selected item                                   *
*                                                                       *
*  Proposito:  Dibuja el item seleccionado                              *
*                                                                       *
*  Passed:     char item - Nº of item                                   *
*              char menu - The menu it belongs                          *
*  Argumento:  char item - Posicion del item                            *
*              char menu - El menu al que pertenece                     *  
*  Returned:   None                                                     *
*  Retorno:    Nada                                                     *
*  Note:                                                                *
*                                                                       *
*                                                                       *
************************************************************************/
void menu_select(char item, char menu)
{
   switch (menu)
   {
      case 0:
      {
         switch (item)
         {
            case 1:
            {
               menu_drawmain();
               
               glcd_bar(0, 17, 127, 17, 9, ON);    //selected
               glcd_text57(0, 14, MENU1, 1, OFF);
               menu_entrar(14,OFF);
               
               glcd_update();
               break;
            }
            case 2:
            {
               menu_drawmain();  
               
               glcd_bar(0, 26, 127, 26, 9, ON);    //selected
               glcd_text57(0, 23, MENU2, 1, OFF);
               glcd_text57(100, 23, UniTemp, 1, OFF);
               menu_entrar(23,OFF);
               
               glcd_update();
               break;
            }
            case 3:
            {
               menu_drawmain();  
              
               glcd_bar(0, 35, 127, 35, 9, ON); //selected
               glcd_text57(0, 32, MENU3, 1, OFF);
               glcd_text57(100, 32, UniPres, 1, OFF);
               menu_entrar(32,OFF);
               
              
               glcd_update();
               break;
            }
            case 4:
            {
               menu_drawmain(); 
               
               glcd_bar(0, 44, 127, 44, 9, ON); //selected
               glcd_text57(0, 41, MENU4, 1, OFF);
               glcd_text57(100, 41, UniVel, 1, OFF);
               menu_entrar(41,OFF);
               
         
               
               glcd_update();
               break;
            }            
         }
         break;
      }
      case 1:
      {
         
         break;
      }
      case 2:
      {
         switch (item)
         {
            case 1:
            {
               glcd_fillScreen(OFF);
               glcd_text57(0, 0, MENU2, 1, ON)   ; //Titulo
               glcd_bar(0, 9, 127, 9, 2, ON);      //div
               glcd_bar(0, 17, 127, 17, 9, ON);    //selected
               glcd_text57(0, 14, MENU2ITEM1, 1, OFF);
                 
               glcd_text57(0, 23, MENU2ITEM2, 1, ON)   ;
               
               glcd_update();
               break;
            }
            case 2:
            {
               glcd_fillScreen(OFF);
               glcd_text57(0, 0, MENU2, 1, ON);       //Titulo
               glcd_bar(0, 9, 127, 9, 2, ON);         //div
               glcd_text57(0, 14, MENU2ITEM1, 1, ON);
               glcd_bar(0, 26, 127, 26, 9, ON);       //selected
               glcd_text57(0, 23, MENU2ITEM2, 1, OFF);
         
              
               glcd_update();
               break;
            }
            
         }
       break;
      }
      case 3:
      {
         switch (item)
               {
                  case 1:
                  {
                     glcd_fillScreen(OFF);
                     glcd_text57(0, 0, MENU3, 1, ON)   ; //Titulo
                     glcd_bar(0, 9, 127, 9, 2, ON);      //div
                     glcd_bar(0, 17, 127, 17, 9, ON);    //selected
                     glcd_text57(0, 14, MENU3ITEM1, 1, OFF);
                       
                     glcd_text57(0, 23, MENU3ITEM2, 1, ON)   ;
                     
                     glcd_update();
                     break;
                  }
                  case 2:
                  {
                     glcd_fillScreen(OFF);
                     glcd_text57(0, 0, MENU3, 1, ON);       //Titulo
                     glcd_bar(0, 9, 127, 9, 2, ON);         //div
                     glcd_text57(0, 14, MENU3ITEM1, 1, ON);
                     glcd_bar(0, 26, 127, 26, 9, ON);       //selected
                     glcd_text57(0, 23, MENU3ITEM2, 1, OFF);
               
                    
                     glcd_update();
                     break;
                  }
               }
               break;
      }
      case 4:
      {
         switch (item)
               {
                  case 1:
                  {
                     glcd_fillScreen(OFF);
                     glcd_text57(0, 0, MENU4, 1, ON)   ; //Titulo
                     glcd_bar(0, 9, 127, 9, 2, ON);      //div
                     glcd_bar(0, 17, 127, 17, 9, ON);    //selected
                     glcd_text57(0, 14, MENU4ITEM1, 1, OFF);
                       
                     glcd_text57(0, 23, MENU4ITEM2, 1, ON)   ;
                     glcd_text57(0, 32, MENU4ITEM3, 1, ON)   ;
                     glcd_update();
                     break;
                  }
                  case 2:
                  {
                     glcd_fillScreen(OFF);
                     glcd_text57(0, 0, MENU4, 1, ON);       //Titulo
                     glcd_bar(0, 9, 127, 9, 2, ON);         //div
                     glcd_text57(0, 14, MENU4ITEM1, 1, ON);
                     glcd_bar(0, 26, 127, 26, 9, ON);       //selected
                     glcd_text57(0, 23, MENU4ITEM2, 1, OFF);
                     glcd_text57(0, 32, MENU4ITEM3, 1, ON)   ;
                    
                     glcd_update();
                     break;
                  }
                  case 3:
                  {
                     glcd_fillScreen(OFF);
                     glcd_text57(0, 0, MENU4, 1, ON);       //Titulo
                     glcd_bar(0, 9, 127, 9, 2, ON);         //div
                     glcd_text57(0, 14, MENU4ITEM1, 1, ON);
                    
                     glcd_text57(0, 23, MENU4ITEM2, 1, ON);
                     glcd_bar(0, 35, 127, 35, 9, ON); //selected
                     glcd_text57(0, 32, MENU4ITEM3, 1, OFF)   ;
                    
                     glcd_update();
                     break;
                  }
               }
               break;
      }
   
   }
}

/************************************************************************
*                                                                       *
*  Purpose:    Draw and handle the submenu                              *
*                                                                       *
*  Proposito:  Dibuja y maneja cada submenu                             *
*                                                                       *
*  Passed:     SubMenu position identifier                              *
*  Argumento:  Identificador de posicion del submenu                    *
*  Returned:   None                                                     *
*  Retorno:    Nada                                                     *
*  Note:                                                                *
*                                                                       *
*                                                                       *
************************************************************************/
void menu_submenu(char menu)
{
   switch (menu)
   {
      case 1:
      {  
         menu_timemenu();
         i=0;
         return;
         break;
      }
      case 2:
      {  
         
         glcd_fillScreen(OFF);
         glcd_text57(0, 0, MENU2, 1, ON)   ;
         glcd_bar(0, 9, 127, 9, 2, ON); //div
         glcd_text57(0, 14, MENU2ITEM1, 1, ON)   ;
         glcd_text57(0, 23, MENU2ITEM2, 1, ON)   ;
         glcd_update();
         i = 0;
         break;
      }
      case 3:
      {  
         glcd_fillScreen(OFF);
         glcd_text57(0, 0, MENU3, 1, ON)   ;
         glcd_bar(0, 9, 127, 9, 2, ON); //div
         glcd_text57(0, 14, MENU3ITEM1, 1, ON)   ;
         glcd_text57(0, 23, MENU3ITEM2, 1, ON)   ;
         glcd_update();
         i = 0;
         break;
      }
      case 4:
      {  
         glcd_fillScreen(OFF);
         glcd_text57(0, 0, MENU4, 1, ON)   ;
         glcd_bar(0, 9, 127, 9, 2, ON); //div
         glcd_text57(0, 14, MENU4ITEM1, 1, ON)   ;
         glcd_text57(0, 23, MENU4ITEM2, 1, ON)   ;
         glcd_text57(0, 32, MENU4ITEM3, 1, ON)   ;
         glcd_update();
         i = 0;
         break;
      }
   }
   
   while(1)
   {
      readbuttons();
      if ((up==!PRESSED)&&(down==PRESSED))
      {
         i+=1;
         menu_select(i,menu);
         
      }
      if ((up==PRESSED)&&(down==!PRESSED))
      {
         i-=1;
         menu_select(i,menu);
         
      }
      if (left==PRESSED)
      {
         i = 0;
         
         button = !PRESSED;
         break;
      }
      if (enter==PRESSED)
      {
         switch (menu)
         {
            case 2:
            {
               switch (i)
               {
                  case 1:
                  {
                     UniTemp = "C";
                     
                     break;
                  }
                  case 2:
                  {
                     UniTemp = "F";
                     
                     break;
                  }
               }
               break;
            }
            case 3:
            {
               switch (i)
               {
                  case 1:
                  {
                     UniPres = "mBa";
                     
                     break;
                  }
                  case 2:
                  {
                     UniPres = "Pa";                     
                     break;
                  }
               }
               break;
            }
            case 4:
            {
               switch (i)
               {
                  case 1:
                  {
                     UniVel = "Nud";
                     
                     break;
                  }
                  case 2:
                  {
                     UniVel = "km/h";
                     
                     break;
                  }
                  case 3:
                  {
                     UniVel = "m/s"; 
                     break;
                  }
               }
               break;
            }
               
         }
         i=0;
         button = !PRESSED;
         break;
      }
      button = !PRESSED;
   }
   button = !PRESSED;
}

/************************************************************************
*                                                                       *
*  Purpose:    Handles the main menu                                    *
*                                                                       *
*  Proposito:  Maneja el menu principal                                 *
*                                                                       *
*  Passed:     None                                                     *
*  Argumento:  Nada                                                     *
*  Returned:   None                                                     *
*  Retorno:    Nada                                                     *
*  Note:                                                                *
*                                                                       *
*                                                                       *
************************************************************************/
void menu_mainmenu(void)
{
   menu_drawmain();
   while (1)
   {
      readbuttons();
      if (enter==PRESSED)
      {
         return;
      }
      if ((up==!PRESSED)&&(down==PRESSED))
      {
         i+=1;
         menu_select(i,0);
         
      }
      if ((up==PRESSED)&&(down==!PRESSED))
      {
         i-=1;
         menu_select(i,0);
         
      }
      if (right==PRESSED)
      {
         if (i)
         {
            menu_submenu(i);
         }
         if (i==0)
         {
            menu_drawmain();
         }
         
                  
      }
      
      button = !PRESSED;
   }
}

/************************************************************************
*                                                                       *
*  Purpose:    Draw and handles the time config menu                    *
*                                                                       *
*  Proposito:  Dibuja y maneja el menu de configuracion de la hora      *
*                                                                       *
*  Passed:     None                                                     *
*  Argumento:  Nada                                                     *
*  Returned:   None                                                     *
*  Retorno:    Nada                                                     *
*  Note:                                                                *
*                                                                       *
*                                                                       *
************************************************************************/
void menu_timemenu(void)
{
   glcd_fillScreen(OFF);
   glcd_text57(0, 0, MENU1, 1, ON);
   glcd_bar(0, 9, 127, 9, 2, ON); //div
   
   glcd_rect(5, 20, 25, 35, NO, ON);         //DoW
   glcd_text57(9, 24, dow, 1, ON);

   glcd_rect(35, 20, 55, 35, NO, ON);         //Dia   
   glcd_text57(39, 24, dia, 1, ON);
   
   glcd_rect(65, 20, 85, 35, NO, ON);        //Mes
   glcd_text57(69, 24, mes, 1, ON);
   
   glcd_rect(95, 20, 115, 35, NO, ON);        //Año
   glcd_text57(99, 24, ao, 1, ON);
   
   
   glcd_rect(5, 45, 25, 60, NO, ON);         //Hora
   glcd_text57(9, 49, hora, 1, ON);
   
   glcd_rect(35, 45, 55, 60, NO, ON);        //Minuto
   glcd_text57(39, 49, min, 1, ON);
   
   glcd_rect(65, 45, 85, 60, NO, ON);        // AM/PM.
   glcd_text57(69, 49, aop, 1, ON);
   
   
   glcd_update();
   

   
   while (1)
   {

      readbuttons();
      if (right==PRESSED)
      {
         control+=1;
         if (control>=8)
         {
            control = 1;
         }
      }
      if (left==PRESSED)
      {
         control-=1;
         if (control<=0)
         {
            control = 7;
         }
      }
      if (enter==PRESSED)
      {
         control = 10;
      }
         
         
      switch (control)
      {
         case 1:
         {
            glcd_rect(65, 45, 85, 60, YES, OFF); 
            glcd_rect(65, 45, 85, 60, NO, ON);        // AM/PM.
            glcd_text57(69, 49, aop, 1, ON);
            

            glcd_rect(5, 20, 25, 35, YES, ON);         //DoW
            glcd_text57(9, 24, dow, 1, OFF);

            glcd_rect(35, 20, 55, 35, YES, OFF); 
            glcd_rect(35, 20, 55, 35, NO, ON);         //Dia     
            glcd_text57(39, 24, dia, 1, ON);
            
            glcd_update();

            if ((up==!PRESSED)&&(down==PRESSED))
            {
               k++;

               if(k>=7)
               {
                  k=0;
               }
             
               switch(k)
               {
                  case 0:
                  {
                     dow[0] = "L";
                     break;
                  }
                  case 1:
                  {
                     dow[0] = "M";
                     break;
                  }
                  case 2:
                  {
                     dow[0] = "I";
                     break;
                  }
                  case 3:
                  {
                     dow[0] = "J";
                     break;
                  }
                  case 4:
                  {
                     dow[0] = "V";
                     break;
                  }
                  case 5:
                  {
                     dow[0] = "S";
                     break;
                  }
                  case 6:
                  {
                     dow[0] = "D";
                     break;
                  }
               }

              
               glcd_rect(5, 20, 25, 35, YES, ON);         //DoW
               
               glcd_text57(9, 24, dow, 1, OFF);
               glcd_update();
            }
            if ((up==PRESSED)&&(down==!PRESSED))
            {
               k--;

               if((k<0)||(k>10))
               {
                  k=6;
               }

               switch(k)
               {
                  case 0:
                  {
                     dow[0] = "L";
                     break;
                  }
                  case 1:
                  {
                     dow[0] = "M";
                     break;
                  }
                  case 2:
                  {
                     dow[0] = "I";
                     break;
                  }
                  case 3:
                  {
                     dow[0] = "J";
                     break;
                  }
                  case 4:
                  {
                     dow[0] = "V";
                     break;
                  }
                  case 5:
                  {
                     dow[0] = "S";
                     break;
                  }
                  case 6:
                  {
                     dow[0] = "D";
                     break;
                  }
               }

               glcd_rect(5, 20, 25, 35, YES, ON);         //DoW
               
               glcd_text57(9, 24, dow, 1, OFF);
               glcd_update();
      
            }



            break;
         }
         case 2:           // DIA
         {
                        
            glcd_rect(5, 20, 25, 35, YES, OFF); 
            glcd_rect(5, 20, 25, 35, NO, ON);        // DoW
            glcd_text57(9, 24, dow, 1, ON);
            

            glcd_rect(35, 20, 55, 35, YES, ON);         //Dia
            glcd_text57(39, 24, dia, 1, OFF);

            glcd_rect(65, 20, 85, 35, YES, OFF); 
            glcd_rect(65, 20, 85, 35, NO, ON);         //Mes        
            glcd_text57(69, 24, mes, 1, ON);
            
            glcd_update();

            if ((up==!PRESSED)&&(down==PRESSED))
            {  
               int8 auxdia;
               auxdia = atoi(dia);
               auxdia -=1;
               if (auxdia==0)
               {
                  if (atoi(mes)==2)
                  {
                     auxdia = 28;
                  }
                  else
                  {
                     auxdia = 31;
                  }
               }
               itoa(auxdia,10,dia);
               glcd_rect(35, 20, 55, 35, YES, ON);         //Dia
               
               glcd_text57(39, 24, dia, 1, OFF);
               glcd_update();
            }
            if ((up==PRESSED)&&(down==!PRESSED))
            {
               int8 auxdia;
               auxdia = atoi(dia);
               auxdia +=1;
               if (atoi(mes)==2)
               {
                  if(auxdia==29)
                  {
                     auxdia = 1;
                  }
               }
               if (auxdia==32)
               {
                  auxdia = 1;
               }
               itoa(auxdia,10,dia);
               glcd_rect(35, 20, 55, 35, YES, ON);         //Dia
               
               glcd_text57(39, 24, dia, 1, OFF);
               glcd_update();
      
            }
            break;
         }
         case 3:           // MES
         {
            
            glcd_rect(35, 20, 55, 35, YES, OFF); 
            glcd_rect(35, 20, 55, 35, NO, ON);         //Dia
            glcd_text57(39, 24, dia, 1, ON);
            
            glcd_rect(65, 20, 85, 35, YES, ON);         //Mes
            glcd_text57(69, 24, mes, 1, OFF);

            glcd_rect(95, 20, 115, 35, YES, OFF); 
            glcd_rect(95, 20, 115, 35, NO, ON);         //Año
            glcd_text57(99, 24, ao, 1, ON);
            
            glcd_update();

            if ((up==!PRESSED)&&(down==PRESSED))
            {  
               int8 auxmes;
               auxmes = atoi(mes);
               auxmes -=1;
               if (auxmes==0)
               {
                  auxmes = 12;
               }
               itoa(auxmes,10,mes);
               glcd_rect(65, 20, 85, 35, YES, ON);         //Mes
               
               glcd_text57(69, 24, mes, 1, OFF);
               glcd_update();
            }
            if ((up==PRESSED)&&(down==!PRESSED))
            {
               int8 auxmes;
               auxmes = atoi(mes);
               auxmes +=1;
               if (auxmes==13)
               {
                  auxmes = 1;
               }
               itoa(auxmes,10,mes);
               glcd_rect(65, 20, 85, 35, YES, ON);         //Mes
               
               glcd_text57(69, 24, mes, 1, OFF);
               glcd_update();
      
            }
            break;
         }
         case 4:           // AÑO
         {
            
            glcd_rect(65, 20, 85, 35, YES, OFF); 
            glcd_rect(65, 20, 85, 35, NO, ON);         //Mes         
            glcd_text57(69, 24, mes, 1, ON);
            
            glcd_rect(95, 20, 115, 35, YES, ON);         //Año
            glcd_text57(99, 24, ao, 1, OFF);

            glcd_rect(5, 45, 25, 60, YES, OFF); 
            glcd_rect(5, 45, 25, 60, NO, ON);         //Hora
            glcd_text57(9, 49, hora, 1, ON);

            glcd_update();
            
            if ((up==!PRESSED)&&(down==PRESSED))
            {  
               int8 auxao;
               auxao = atoi(ao);
               auxao -=1;
               if (auxao==0)
               {
                  auxao = 99;
               }
               itoa(auxao,10,ao);

               glcd_rect(95, 20, 115, 35, YES, ON);         //Año
               glcd_text57(99, 24, ao, 1, OFF);

               glcd_update();
            }
            if ((up==PRESSED)&&(down==!PRESSED))
            {
               int8 auxao;
               auxao = atoi(ao);
               auxao +=1;
               if (auxao==99)
               {
                  auxao = 0;
               }
               itoa(auxao,10,ao);
               glcd_rect(95, 20, 115, 35, YES, ON);         //Año
               
               glcd_text57(99, 24, ao, 1, OFF);
               glcd_update();
      
            }
            break;
         }
         case 5:           // HORA
         {
            
            glcd_rect(95, 20, 115, 35, YES, OFF); 
            glcd_rect(95, 20, 115, 35, NO, ON);         //Año
            glcd_text57(99, 24, ao, 1, ON);
            
            glcd_rect(5, 45, 25, 60, YES, ON);         //Hora               
            glcd_text57(9, 49, hora, 1, OFF);
              
            glcd_rect(35, 45, 55, 60, YES, OFF); 
            glcd_rect(35, 45, 55, 60, NO, ON);        //Minuto
            glcd_text57(39, 49, min, 1, ON);
            
            glcd_update();      
            
            if ((up==!PRESSED)&&(down==PRESSED))
            {  
               int8 auxhora;
               auxhora = atoi(hora);
               auxhora -=1;
               
               if ((aop[0]==0x41)||(aop[0]==0x50))
               {
                  if (auxhora<=0)
                  {
                     auxhora = 12;
                  }                  
               }
               else
               {
                  if ((auxhora<0)||(auxhora>50))
                  {
                     auxhora = 23;
                  }                  
               }
                  
               
               itoa(auxhora,10,hora);
               
               glcd_rect(5, 45, 25, 60, YES, ON);         //Hora               
               glcd_text57(9, 49, hora, 1, OFF);

               glcd_update();
            }
            if ((up==PRESSED)&&(down==!PRESSED))
            {
               int8 auxhora;
               auxhora = atoi(hora);
               auxhora +=1;
               if ((aop[0]==0x41)||(aop[0]==0x50))
               {
                  if (auxhora>12)
                  {
                     auxhora = 1;
                  }
               }
               else
               {
                  if (auxhora>23)
                  {
                     auxhora = 0;
                  }
               }

               itoa(auxhora,10,hora);
               glcd_rect(5, 45, 25, 60, YES, ON);         //Hora
               
               glcd_text57(9, 49, hora, 1, OFF);
               glcd_update();
      
            }  
            
            break;
         }
         case 6:           // MINUTOS
         {
            glcd_rect(5, 45, 25, 60, YES, OFF); 
            glcd_rect(5, 45, 25, 60, NO, ON);         //Hora
            glcd_text57(9, 49, hora, 1, ON);
            
            glcd_rect(35, 45, 55, 60, YES, ON);        //Minuto
            glcd_text57(39, 49, min, 1, OFF);
            

            glcd_rect(65, 45, 85, 60, YES, OFF); 
            glcd_rect(65, 45, 85, 60, NO, ON);        // AM/PM.
            glcd_text57(69, 49, aop, 1, ON);
                                 
            glcd_update();

            if ((up==!PRESSED)&&(down==PRESSED))
            {  
               int8 auxmin;
               auxmin = atoi(min);
               
               if (auxmin==0)
               {
                  auxmin = 60;
               }
               auxmin -=1;
               itoa(auxmin,10,min);
               
               glcd_rect(35, 45, 55, 60, YES, ON);        //Minuto
               glcd_text57(39, 49, min, 1, OFF);
               glcd_update();
            }
            if ((up==PRESSED)&&(down==!PRESSED))
            {
               int8 auxmin;
               auxmin = atoi(min);
               auxmin +=1;
               if (auxmin>=60)
               {
                  auxmin = 0;
               }
               itoa(auxmin,10,min);
               
               glcd_rect(35, 45, 55, 60, YES, ON);        //Minuto
               glcd_text57(39, 49, min, 1, OFF);
               glcd_update();
      
            }
            break;
         }
         case 7:           // AM - PM                         
         {
            
            glcd_rect(35, 45, 55, 60, YES, OFF); 
            glcd_rect(35, 45, 55, 60, NO, ON);        //Minuto
            glcd_text57(39, 49, min, 1, ON);
            
            glcd_rect(65, 45, 85, 60, YES, ON);        //AM PM
            glcd_text57(69, 49, aop, 1, OFF);

            glcd_rect(5, 20, 25, 35, YES, OFF); 
            glcd_rect(5, 20, 25, 35, NO, ON);         //DoW
            glcd_text57(9, 24, dow, 1, ON);
            
            glcd_update();
            
            if ((up==!PRESSED)&&(down==PRESSED))
            {
               if (aop[0]==0x41)
               {
                  aop[0] = "P";
               }
               else if (aop[0]==0x50)
               {
                  aop[0] = "M";
               }
               else if (aop[0]==0x4D)
               {
                  aop[0] = "A";
               }
               
               glcd_rect(65, 45, 85, 60, YES, ON);        //Minuto
               glcd_text57(69, 49, aop, 1, OFF);
               
               glcd_update();
      
            }
            if ((up==PRESSED)&&(down==!PRESSED))
            {
               if (aop[0]==0x41)
               {
                  aop[0] = "M";
               }
               else if (aop[0]==0x50)
               {
                  aop[0] = "A";
               }
               else if (aop[0]==0x4D)
               {
                  aop[0] = "P";
               }
               
               glcd_rect(65, 45, 85, 60, YES, ON);        //Minuto
               glcd_text57(69, 49, aop, 1, OFF);
               
               glcd_update();
      
            }
            
            break;                                                    
         }
         case 10:          // SET                        
         {
            glcd_fillscreen(OFF);
            glcd_text57(0, 0, MENU1, 1, ON);
            glcd_bar(0, 9, 127, 9, 2, ON); //div
            
            glcd_rect(5, 20, 25, 35, YES, ON);         //DoW
            glcd_text57(9, 24, dow, 1, OFF);

            glcd_rect(5, 20, 25, 35, NO, ON);         //Dia    
            glcd_text57(9, 24, dia, 1, ON);
            
            glcd_rect(35, 20, 55, 35, NO, ON);        //Mes
            glcd_text57(39, 24, mes, 1, ON);
            
            glcd_rect(65, 20, 85, 35, NO, ON);        //Año
            glcd_text57(69, 24, ao, 1, ON);
            
            
            glcd_rect(5, 45, 25, 60, NO, ON);         //Hora
            glcd_text57(9, 49, hora, 1, ON);
            
            glcd_rect(35, 45, 55, 60, NO, ON);        //Minuto
            glcd_text57(39, 49, min, 1, ON);
            
            glcd_rect(65, 45, 85, 60, NO, ON);        // AM/PM.
            glcd_text57(69, 49, aop, 1, ON);
            
            glcd_update();
            
            break;                                                    
         }
                                                                     
      }
      
      
                                                                         
      button = !PRESSED;
      if (control==10)
      {
         /*GRABAR EN RTC*/
         ds1307_set_date_time(atoi(dia),atoi(mes),atoi(ao),(k+1),atoi(hora),atoi(min),0,aop[0]);
         return;
      }

   }
}

/************************************************************************
*                                                                       *
*  Purpose:    Draw main menu                                           *
*                                                                       *
*  Proposito:  Dibuja el menu principal                                 *
*                                                                       *
*  Passed:     None                                                     *
*  Argumento:  Nada                                                     *
*  Returned:   None                                                     *
*  Retorno:    Nada                                                     *
*  Note:                                                                *
*                                                                       *
*                                                                       *
************************************************************************/
void menu_drawmain(void)
{
   glcd_fillScreen(OFF);
   glcd_text57(0, 0, MENUTITLE, 1, ON);
   glcd_bar(0, 9, 127, 9, 2, ON); //div
   glcd_text57(0, 14, MENU1, 1, ON);
   menu_entrar(14,ON);
   glcd_text57(0, 23, MENU2, 1, ON);
   glcd_text57(100, 23, UniTemp, 1, ON);
   menu_entrar(23,ON);
   glcd_text57(0, 32, MENU3, 1, ON);
   glcd_text57(100, 32, UniPres, 1, ON);
   menu_entrar(32,ON);
   glcd_text57(0, 41, MENU4, 1, ON);
   glcd_text57(100, 41, UniVel, 1, ON);
   menu_entrar(41,ON);
   glcd_update();
}
