/////////////////////////////////////////////////////////////////////////
////                           Show.c                                ////
////                                                                 ////
//// Ester archivo contiene las funciones para implementar la        ////
//// impresion de imagenes en una pantalla lcd grafica de            ////
//// 128x64 con controlador KS0108                            ////
/////////////////////////////////////////////////////////////////////////

#include "../Libs/imagenes.c"

/************************************************************************
*  Function Prototypes                                                  *
************************************************************************/
void glcd_text_sec(char sec, char line, char* textptr, int1 color);
void draw_sect(char sector = 0);
void glcd_shownub();
void glcd_showsun();
void glcd_showlogo();
void glcd_loadscreen();




void glcd_showlogo() 
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
         glcd_writeByte(cs,logo[n]);  // Turn pixels on or off 
         //delay_us(5); 
        n++; 
      } 
   } 
   i = 0;
}    

void glcd_showimg_xy(char *img,int x, int y, int1 inverse)
{

   unsigned int i, j, k;
   unsigned int16 count=2;
   int width  = img[0];
         int height = img[1];
         if(inverse)
         {
            for(i=0;i<height;i+=8)
            {   
               for(j=0;j<width;j++)
               {
                  for(k=0;k<8;k++)
                  { 
                     if(~bit_test(img[count],k))
                     {
                        glcd_pixel(x+j,y+i+k,ON);
                     }
                     else
                     {
                        glcd_pixel(x+j,y+i+k,OFF);
                     }
                  }
                  count++;
               }
            }
         }
         else
         {
            for(i=0;i<height;i+=8)
            {   
               for(j=0;j<width;j++)
               {
                  for(k=0;k<8;k++)
                  { 
                     if(bit_test(img[count],k))
                     {
                        glcd_pixel(x+j,y+i+k,ON);
                     }
                     else
                     {
                        //glcd_pixel(x+j,y+i+k,OFF);
                     }
                  }
                  count++;
               }
            }
         }
   //glcd_fillScreen(OFF);                        //Clears the screen (opt.)
   
} 

void glcd_loadscreen()
{
   int g;
   for (g=0;g<63;g++)
   {  
   
      char h,k;
      h = (2*g);
      k = (2*g)+1;
      
      char l[] = "Iniciando X-Weather";
      
       char n[] = "Iniciando Sensores";
      glcd_bar(h, 5, h, 15, 1, ON);
      glcd_bar(k, 5, k, 15, 1, ON);
      glcd_text57(5, 7, l, 1, OFF) ;
      
      
      
      glcd_update();
      
      
   }
   glcd_rect(0,6,127,15,YES,ON);
   for (g=0;g<63;g++)
   {  
   
      char h,k;
      h = (2*g);
      k = (2*g)+1;
      
      
      
       char n[] = "Iniciando Sensores";
       
      glcd_bar(h, 5, h, 15, 1, OFF);
      glcd_bar(k, 5, k, 15, 1, OFF);
      glcd_text57(5, 7, n, 1, ON) ;
      
      
      
      glcd_update();
      
      
   }

}
   
void draw_sect(char sector)
{
   switch (sector)
   {
      case 1:        //Sec1
      {
         glcd_rect(0, 0, 62, 30, YES, ON);   
         glcd_rect(0, 0, 62, 30, NO, OFF);
         break;
      }
      case 2:        //Sec2
      {
         glcd_rect(63, 0, 127, 30, NO, ON);
         break;
      }
      case 3:        //Sec3
      {
         glcd_rect(0, 31, 62, 63, NO, ON);
         break;
      }
      case 4:        //Sec4
      {
         glcd_rect(63, 31, 127, 63, YES, ON);
         glcd_rect(63, 31, 127, 63, NO, OFF);
         break;
      }
      case 0:
      {
         glcd_rect(0, 0, 62, 30, YES, ON);   
         glcd_rect(0, 0, 62, 30, NO, OFF);

         glcd_rect(63, 0, 127, 30, NO, ON);
         glcd_rect(64, 1, 126, 29, YES, OFF);

         glcd_rect(0, 31, 62, 63, NO, ON);
         glcd_rect(1, 32, 61, 62, YES, OFF);

         glcd_rect(63, 31, 127, 63, YES, ON);
         glcd_rect(63, 31, 127, 63, NO, OFF);
         break;
      }
   }   
}

void glcd_text_sec(char sec, char line, char *textptr, int1 color)
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
         glcd_text57(37, 34+ly, textptr, 1, color);
         //Comienza en 37 por la imagen.
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


