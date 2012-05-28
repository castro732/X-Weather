#include "show.h"

/*DEFINICIONES*/  


#define FAST_GLCD





/*FIN**DEFINICIONES*/  


/****/


/*VARIABLES**GLOBALES*/




/*FIN**VARIBLES**GLOBALES*/



/****/


/**INCLUDES**/

#include "HDM64GS12.c"
#include "graphics.c"
#include <stdlib.h>
#include <string.h>



/*FIN**INCLUDES*/


/****/




/*FUNCIONES*/


/*FIN**FUNCIONES*/

void glcd_text_sec(char sec, char line, char* textptr, int1 color);
void draw_sect();



void glcd_showimage1() 
{ 
  int16 n=0; 
  int i,j; 
  int1 cs=0; 
   // Loop through the vertical pages 
   for(i = 0; i <8; ++i) 
   { 
      output_low(GLCD_DI);                      // Set for instruction 
      glcd_writeByte(GLCD_LEFT, 0b01000000);    // Set horizontal address to 0 
      glcd_writeByte(GLCD_RIGHT, 0b01000000); 
      glcd_writeByte(GLCD_LEFT, i | 0b10111000);// Set page address 
      glcd_writeByte(GLCD_RIGHT, i | 0b10111000); 
      output_high(GLCD_DI);                     // Set for data 

      // Loop through the horizontal sections 
      for(j = 0; j < 128;++j) 
      { 
         if(j<64) cs=GLCD_LEFT;else cs=GLCD_RIGHT; 
         glcd_writeByte(cs,nub[n]);  // Turn pixels on or off 
         //delay_us(5); 
        n++; 
      } 
   } 
}    

void glcd_showimage2() 
{ 
  int16 n=0; 
  int i,j; 
  int1 cs=0; 
   // Loop through the vertical pages 
   for(i = 0; i <8; ++i) 
   { 
      output_low(GLCD_DI);                      // Set for instruction 
      glcd_writeByte(GLCD_LEFT, 0b01000000);    // Set horizontal address to 0 
      glcd_writeByte(GLCD_RIGHT, 0b01000000); 
      glcd_writeByte(GLCD_LEFT, i | 0b10111000);// Set page address 
      glcd_writeByte(GLCD_RIGHT, i | 0b10111000); 
      output_high(GLCD_DI);                     // Set for data 

      // Loop through the horizontal sections 
      for(j = 0; j < 128;++j) 
      { 
         if(j<64) cs=GLCD_LEFT;else cs=GLCD_RIGHT; 
         glcd_writeByte(cs,sun[n]);  // Turn pixels on or off 
         //delay_us(5); 
        n++; 
      } 
   } 
}    

void main()
{

   
   glcd_init(ON);  

   delay_ms(100);
   
   draw_sect();
   int t = 234;
   char temp[10];
   sprintf(temp,"%3.1w",t);
   glcd_text_sec(1,1,temp,OFF);
   glcd_update();
   
  
  /*
  
   while (1)
   {
   int8 i=0;
   for (i=0;i<63;i++)
   {  
   
      char h,k;
      h = (2*i);
      k = (2*i)+1;
      
      char l[] = "Iniciando X-Weather";
      
       char n[] = "Iniciando Sensores";
      glcd_bar(h, 5, h, 15, 1, ON);
      glcd_bar(k, 5, k, 15, 1, ON);
      glcd_text57(5, 7, l, 1, OFF) ;
      
      
      
      glcd_update();
      
      
   }
   glcd_rect(0,6,127,15,YES,ON);
   for (i=0;i<63;i++)
   {  
   
      char h,k;
      h = (2*i);
      k = (2*i)+1;
      
      
      
       char n[] = "Iniciando Sensores";
       
      glcd_bar(h, 5, h, 15, 1, OFF);
      glcd_bar(k, 5, k, 15, 1, OFF);
      glcd_text57(5, 7, n, 1, ON) ;
      
      
      
      glcd_update();
      
      
   }
   break;
   }
   //glcd_showimage2()  ;
   //glcd_update();
   delay_ms(1000);
   glcd_showimage1()  ;
   
   draw_sect();
   
   
   char a[] = "Hola 1234";
   glcd_text_sec(1,1,a,OFF);
   glcd_text_sec(1,2,a,OFF);
   glcd_text_sec(1,3,a,OFF);
   
   glcd_text_sec(2,1,a,ON);
   glcd_text_sec(2,2,a,ON);
   glcd_text_sec(2,3,a,ON);
   
   glcd_text_sec(3,1,a,ON);
   glcd_text_sec(3,2,a,ON);
   glcd_text_sec(3,3,a,ON);
   
   glcd_text_sec(4,1,a,OFF);
   glcd_text_sec(4,2,a,OFF);
   glcd_text_sec(4,3,a,OFF);
   
   glcd_update();
   */
}

void draw_sect(void)
{
   //Sec1
   glcd_rect(0, 0, 62, 30, YES, ON);   
   glcd_rect(0, 0, 62, 30, NO, OFF);
   
   //Sec2
   glcd_rect(63, 0, 127, 30, NO, ON);
   
   //Sec3
   glcd_rect(0, 31, 62, 63, NO, ON);
   
   //Sec4
   glcd_rect(63, 31, 127, 63, YES, ON);
   glcd_rect(63, 31, 127, 63, NO, OFF);
   
}

void glcd_text_sec(char sec, char line, char* textptr, int1 color)
{
   int8 ly=0;
   ly = (line-1)*9;
   switch (sec)
   {
      case 1:
      {
         glcd_text57(3, 3+ly, textptr, 1, color);
         break;
      }
      case 2:
      {
         glcd_text57(66, 3+ly, textptr, 1, color);
         break;
      }
      case 3:
      {
         glcd_text57(3, 34+ly, textptr, 1, color);
         break;
      }
      case 4:
      {
         glcd_text57(66, 34+ly, textptr, 1, color);
         break;
      }
   }
   
}

void glcd_image(long mempointer  /*This is the image location in program memory*/) 
{  int j, i; 
   int page = 0xB8; 
   char chipsel; 
   char buffer[1]; 
   output_low(GLCD_DI);                // Set for instruction 
   glcd_writeByte(GLCD_CS1, 0x40);     // Set the column address to 0 
   glcd_writeByte(GLCD_CS2, 0x40); 
   glcd_writeByte(GLCD_CS1, page);     // Set the page address to 0 
   glcd_writeByte(GLCD_CS2, page); 
   for (j = 0; j < 8; j++, page+=1) 
   {  output_low(GLCD_DI); 
      glcd_writeByte(GLCD_CS1, page); 
      glcd_writeByte(GLCD_CS2, page); 
      for (i = 0; i < 128; i++) 
      { 
         if ( i < 64) 
         { 
            chipsel = GLCD_CS1; 
         } 
         else 
         { 
            chipsel = GLCD_CS2; 
         } 
         read_program_memory(mempointer, buffer, 1); 
         mempointer++; 
         output_high(GLCD_DI); 
         glcd_writeByte(chipsel, *buffer); 
      } 
   } 
}


