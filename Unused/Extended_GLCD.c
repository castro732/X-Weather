/////////////////////////////////////////////////////////////////////////
////                       Extended_GLCD.c                           ////
////                                                                 ////
////                             by                                  ////
////                     SAINT (Caner ERSiL)                         ////
////                                                                 ////
////             !!! PLEASE READ THIS INFO FIRST !!!                 ////
////                                                                 ////
//// This file contains drivers for using a KS0108 based 128x64 pixel////
//// GLCD. This "Extended_GLCD.c" driver based on, "GLCD.c" driver   ////
//// can be found in CCS installation folder. The driver treats the  ////
//// upper left pixel as (0,0).                                      ////
//                                                                     //
// ! DON'T FORGET TO CHANGE YOUR PIN CONFIGURATION FOR YOUR CIRCUIT !  //
//                                                                     //
/////////////////////////////////////////////////////////////////////////
////                                                                 ////
////                 The NEW functions added by me:                  ////
////                                                                 ////
////  glcd_standby(command)                                          ////
////  glcd_3Dbox(x,y,width,height,depth,color)                       ////
////  glcd_circle_seg(x,y,radius,angle_start,angle_end,color)        ////
////  glcd_image(inverse)                                            ////
////  glcd_image_xy(x,y,inverse)                                     ////
////                                                                 ////
////     !!         YOU CAN FIND DETAILED DESCRIPTIONS        !!     ////
////     !! AND "HOW TO USE" INFORMATIONS ABOVE THE FUNCTIONS !!     ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////                                                                 ////
////            Original(untouched) GLCD functions are:              ////
////                                                                 ////
////  glcd_init(mode)                                                ////
////  glcd_pixel(x,y,color)                                          ////
////  glcd_line(x1,y1,x2,y2,color)                                   ////
////  glcd_rect(x1,y1,x2,y2,fill,color)                              ////
////  glcd_bar(x1,y1,x2,y2,width,color)                              ////
////  glcd_circle(x,y,radius,fill,color)                             ////
////  glcd_text57(x,y,textptr,size,color)                            ////
////  glcd_fillScreen(color)                                         ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2003 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way. :)           ////
/////////////////////////////////////////////////////////////////////////

#include <math.h>
#include "../Bitmap.h"

#ifndef GLCD_CS1
#define GLCD_CS1 PIN_B3    // Chip Selection 1
#endif

#ifndef GLCD_CS2
#define GLCD_CS2 PIN_B4    // Chip Selection 2
#endif

#ifndef GLCD_DI
#define GLCD_DI  PIN_B0    // Data or Instruction input
#endif

#ifndef GLCD_RW
#define GLCD_RW  PIN_B1    // Read/Write
#endif

#ifndef GLCD_E
#define GLCD_E   PIN_B2    // Enable
#endif

#ifndef GLCD_RST
#define GLCD_RST PIN_C0    // Reset
#endif

#define DATAIN   INPUT_D   // Data input port  (Should be same port as DATAOUT)
#define DATAOUT  OUTPUT_D  // Data output port (Should be same port as DATAIN)

#define GLCD_WIDTH 128     // Used for text wrapping by glcd_text57 function

#define ON  1
#define OFF 0

#define YES 1
#define NO  0

BYTE glcd_readByte(BYTE chip);
void glcd_writeByte(BYTE chip, BYTE data);
void glcd_fillScreen(int1 color);

const BYTE TEXT[51][5] ={0x00, 0x00, 0x00, 0x00, 0x00, // SPACE
                         0x00, 0x00, 0x5F, 0x00, 0x00, // !
                         0x00, 0x03, 0x00, 0x03, 0x00, // "
                         0x14, 0x3E, 0x14, 0x3E, 0x14, // #
                         0x24, 0x2A, 0x7F, 0x2A, 0x12, // $
                         0x43, 0x33, 0x08, 0x66, 0x61, // %
                         0x36, 0x49, 0x55, 0x22, 0x50, // &
                         0x00, 0x05, 0x03, 0x00, 0x00, // '
                         0x00, 0x1C, 0x22, 0x41, 0x00, // (
                         0x00, 0x41, 0x22, 0x1C, 0x00, // )
                         0x14, 0x08, 0x3E, 0x08, 0x14, // *
                         0x08, 0x08, 0x3E, 0x08, 0x08, // +
                         0x00, 0x50, 0x30, 0x00, 0x00, // ,
                         0x08, 0x08, 0x08, 0x08, 0x08, // -
                         0x00, 0x60, 0x60, 0x00, 0x00, // .
                         0x20, 0x10, 0x08, 0x04, 0x02, // /
                         0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
                         0x04, 0x02, 0x7F, 0x00, 0x00, // 1
                         0x42, 0x61, 0x51, 0x49, 0x46, // 2
                         0x22, 0x41, 0x49, 0x49, 0x36, // 3
                         0x18, 0x14, 0x12, 0x7F, 0x10, // 4
                         0x27, 0x45, 0x45, 0x45, 0x39, // 5
                         0x3E, 0x49, 0x49, 0x49, 0x32, // 6
                         0x01, 0x01, 0x71, 0x09, 0x07, // 7
                         0x36, 0x49, 0x49, 0x49, 0x36, // 8
                         0x26, 0x49, 0x49, 0x49, 0x3E, // 9
                         0x00, 0x36, 0x36, 0x00, 0x00, // :
                         0x00, 0x56, 0x36, 0x00, 0x00, // ;
                         0x08, 0x14, 0x22, 0x41, 0x00, // <
                         0x14, 0x14, 0x14, 0x14, 0x14, // =
                         0x00, 0x41, 0x22, 0x14, 0x08, // >
                         0x02, 0x01, 0x51, 0x09, 0x06, // ?
                         0x3E, 0x41, 0x59, 0x55, 0x5E, // @
                         0x7E, 0x09, 0x09, 0x09, 0x7E, // A
                         0x7F, 0x49, 0x49, 0x49, 0x36, // B
                         0x3E, 0x41, 0x41, 0x41, 0x22, // C
                         0x7F, 0x41, 0x41, 0x41, 0x3E, // D
                         0x7F, 0x49, 0x49, 0x49, 0x41, // E
                         0x7F, 0x09, 0x09, 0x09, 0x01, // F
                         0x3E, 0x41, 0x41, 0x49, 0x3A, // G
                         0x7F, 0x08, 0x08, 0x08, 0x7F, // H
                         0x00, 0x41, 0x7F, 0x41, 0x00, // I
                         0x30, 0x40, 0x40, 0x40, 0x3F, // J
                         0x7F, 0x08, 0x14, 0x22, 0x41, // K
                         0x7F, 0x40, 0x40, 0x40, 0x40, // L
                         0x7F, 0x02, 0x0C, 0x02, 0x7F, // M
                         0x7F, 0x02, 0x04, 0x08, 0x7F, // N
                         0x3E, 0x41, 0x41, 0x41, 0x3E, // O
                         0x7F, 0x09, 0x09, 0x09, 0x06, // P
                         0x1E, 0x21, 0x21, 0x21, 0x5E, // Q
                         0x7F, 0x09, 0x09, 0x09, 0x76};// R

const BYTE TEXT2[44][5]={0x26, 0x49, 0x49, 0x49, 0x32, // S
                         0x01, 0x01, 0x7F, 0x01, 0x01, // T
                         0x3F, 0x40, 0x40, 0x40, 0x3F, // U
                         0x1F, 0x20, 0x40, 0x20, 0x1F, // V
                         0x7F, 0x20, 0x10, 0x20, 0x7F, // W
                         0x41, 0x22, 0x1C, 0x22, 0x41, // X
                         0x07, 0x08, 0x70, 0x08, 0x07, // Y
                         0x61, 0x51, 0x49, 0x45, 0x43, // Z
                         0x00, 0x7F, 0x41, 0x00, 0x00, // [
                         0x02, 0x04, 0x08, 0x10, 0x20, // \
                         0x00, 0x00, 0x41, 0x7F, 0x00, // ]
                         0x04, 0x02, 0x01, 0x02, 0x04, // ^
                         0x40, 0x40, 0x40, 0x40, 0x40, // _
                         0x00, 0x01, 0x02, 0x04, 0x00, // `
                         0x20, 0x54, 0x54, 0x54, 0x78, // a
                         0x7F, 0x44, 0x44, 0x44, 0x38, // b
                         0x38, 0x44, 0x44, 0x44, 0x44, // c
                         0x38, 0x44, 0x44, 0x44, 0x7F, // d
                         0x38, 0x54, 0x54, 0x54, 0x18, // e
                         0x04, 0x04, 0x7E, 0x05, 0x05, // f
                         0x08, 0x54, 0x54, 0x54, 0x3C, // g
                         0x7F, 0x08, 0x04, 0x04, 0x78, // h
                         0x00, 0x44, 0x7D, 0x40, 0x00, // i
                         0x20, 0x40, 0x44, 0x3D, 0x00, // j
                         0x7F, 0x10, 0x28, 0x44, 0x00, // k
                         0x00, 0x41, 0x7F, 0x40, 0x00, // l
                         0x7C, 0x04, 0x78, 0x04, 0x78, // m
                         0x7C, 0x08, 0x04, 0x04, 0x78, // n
                         0x38, 0x44, 0x44, 0x44, 0x38, // o
                         0x7C, 0x14, 0x14, 0x14, 0x08, // p
                         0x08, 0x14, 0x14, 0x14, 0x7C, // q
                         0x00, 0x7C, 0x08, 0x04, 0x04, // r
                         0x48, 0x54, 0x54, 0x54, 0x20, // s
                         0x04, 0x04, 0x3F, 0x44, 0x44, // t
                         0x3C, 0x40, 0x40, 0x20, 0x7C, // u
                         0x1C, 0x20, 0x40, 0x20, 0x1C, // v
                         0x3C, 0x40, 0x30, 0x40, 0x3C, // w
                         0x44, 0x28, 0x10, 0x28, 0x44, // x
                         0x0C, 0x50, 0x50, 0x50, 0x3C, // y
                         0x44, 0x64, 0x54, 0x4C, 0x44, // z
                         0x00, 0x08, 0x36, 0x41, 0x41, // {
                         0x00, 0x00, 0x7F, 0x00, 0x00, // |
                         0x41, 0x41, 0x36, 0x08, 0x00, // }
                         0x02, 0x01, 0x02, 0x04, 0x02};// ~


//****************************************************************************//
// Purpose:       Initialize a graphic LCD. This must be called before any    //
//                other glcd function is used.                                //
// Inputs:        The initialization mode                                     //
//                OFF - Turns the LCD off                                     //
//                ON  - Turns the LCD on                                      //
//****************************************************************************//
void glcd_init(int1 mode)
{
   // Initialze some pins
   output_high(GLCD_RST);
   output_low(GLCD_E);
   output_low(GLCD_CS1);
   output_low(GLCD_CS2);

   output_low(GLCD_DI);                // Set for instruction
   glcd_writeByte(GLCD_CS1, 0xC0);     // Specify first RAM line at the top
   glcd_writeByte(GLCD_CS2, 0xC0);     //  of the screen
   glcd_writeByte(GLCD_CS1, 0x40);     // Set the column address to 0
   glcd_writeByte(GLCD_CS2, 0x40);
   glcd_writeByte(GLCD_CS1, 0xB8);     // Set the page address to 0
   glcd_writeByte(GLCD_CS2, 0xB8);
   if(mode == ON)
   {
      glcd_writeByte(GLCD_CS1, 0x3F);  // Turn the display on
      glcd_writeByte(GLCD_CS2, 0x3F);
   }
   else
   {
      glcd_writeByte(GLCD_CS1, 0x3E);  // Turn the display off
      glcd_writeByte(GLCD_CS2, 0x3E);
   }

   glcd_fillScreen(OFF);               // Clear the display
}

//****************************************************************************//
// Purpose:       Turns ON-OFF graphic LCD. Good solution on system standby   //
//                GLCD does not clear its memory map on OFF command. So last  //
//                screen shows up when ON command send to GLCD                //
// Inputs:        OFF - Turns the LCD off                                     //
//                ON  - Turns the LCD on                                      //
//****************************************************************************//
void glcd_standby(int1 command)
{  
   output_low(GLCD_E);
   output_low(GLCD_CS1);
   output_low(GLCD_CS2);
   output_low(GLCD_DI);
   if (command)
   {   
      glcd_writeByte(GLCD_CS1,0x3F);
      glcd_writeByte(GLCD_CS2,0x3F);
   }
   else
   {
      glcd_writeByte(GLCD_CS1,0x3E);
      glcd_writeByte(GLCD_CS2,0x3E);   
   }
}


//****************************************************************************//
// Purpose:       Turn a pixel on a graphic LCD on or off                     //
// Inputs:        x - the x coordinate of the pixel                           //
//                y - the y coordinate of the pixel                           //
//                color - ON or OFF                                           //
// Output:        1 if coordinate out of range, 0 if in range                 //
//****************************************************************************//
void glcd_pixel(int x, int y, int1 color)
{
   BYTE data;
   BYTE chip = GLCD_CS1;  // Stores which chip to use on the LCD

   if(x > 63)  // Check for first or second display area
   {
      x -= 64;
      chip = GLCD_CS2;
   }

   output_low(GLCD_DI);                                     // Set for instruction
   bit_clear(x,7);                                          // Clear the MSB. Part of an instruction code
   bit_set(x,6);                                            // Set bit 6. Also part of an instruction code
   glcd_writeByte(chip, x);                                 // Set the horizontal address
   glcd_writeByte(chip, (y/8 & 0b10111111) | 0b10111000);   // Set the vertical page address
   output_high(GLCD_DI);                                    // Set for data
   data = glcd_readByte(chip);

   if(color == ON)
      bit_set(data, y%8);        // Turn the pixel on
   else                          // or
      bit_clear(data, y%8);      // turn the pixel off
   output_low(GLCD_DI);          // Set for instruction
   glcd_writeByte(chip, x);      // Set the horizontal address
   output_high(GLCD_DI);         // Set for data
   glcd_writeByte(chip, data);   // Write the pixel data
}


//****************************************************************************//
// Purpose:       Draw a line on a graphic LCD using Bresenham's              //
//                line drawing algorithm                                      //   
// Inputs:        (x1, y1) - the start coordinate                             //
//                (x2, y2) - the end coordinate                               //
//                color - ON or OFF                                           //
// Dependencies:  glcd_pixel()                                                //
//****************************************************************************//
void glcd_line(int x1, int y1, int x2, int y2, int1 color)
{
   signed int  x, y, addx, addy, dx, dy;
   signed long P;
   int i;
   dx = abs((signed int)(x2 - x1));
   dy = abs((signed int)(y2 - y1));
   x = x1;
   y = y1;

   if(x1 > x2)
      addx = -1;
   else
      addx = 1;
   if(y1 > y2)
      addy = -1;
   else
      addy = 1;

   if(dx >= dy)
   {
      P = 2*dy - dx;

      for(i=0; i<=dx; ++i)
      {
         glcd_pixel(x, y, color);

         if(P < 0)
         {
            P += 2*dy;
            x += addx;
         }
         else
         {
            P += 2*dy - 2*dx;
            x += addx;
            y += addy;
         }
      }
   }
   else
   {
      P = 2*dx - dy;

      for(i=0; i<=dy; ++i)
      {
         glcd_pixel(x, y, color);

         if(P < 0)
         {
            P += 2*dx;
            y += addy;
         }
         else
         {
            P += 2*dx - 2*dy;
            x += addx;
            y += addy;
         }
      }
   }
}


//****************************************************************************//
// Purpose:       Draw a rectangle on a graphic LCD                           //
// Inputs:        (x1, y1) - the start coordinate                             //
//                (x2, y2) - the end coordinate                               //
//                fill  - YES or NO                                           //
//                color - ON or OFF                                           //
// Dependencies:  glcd_pixel(), glcd_line()                                   //   
//****************************************************************************//
void glcd_rect(int x1, int y1, int x2, int y2, int fill, int1 color)
{
   if(fill)
   {
      int y, ymax;                          // Find the y min and max
      if(y1 < y2)
      {
         y = y1;
         ymax = y2;
      }
      else
      {
         y = y2;
         ymax = y1;
      }

      for(; y<=ymax; ++y)                    // Draw lines to fill the rectangle
         glcd_line(x1, y, x2, y, color);
   }
   else
   {
      glcd_line(x1, y1, x2, y1, color);      // Draw the 4 sides
      glcd_line(x1, y2, x2, y2, color);
      glcd_line(x1, y1, x1, y2, color);
      glcd_line(x2, y1, x2, y2, color);
   }
}


//****************************************************************************//
// Purpose:       Draw a 3D box on a graphic LCD (Ex: to make a 3D Button)    //
// Inputs:        (x, y) - the start point                                    //
//                (width, height) - size of the box                           //
//                depth  - 3D depth effect ratio of the box (2 or 3 is fine)  //
//                color - ON or OFF                                           //
// Dependencies:  glcd_pixel()                                                //   
//****************************************************************************//
void glcd_3Dbox(int16 x,int16 y,int16 width, int16 height, int depth, int1 color)
{
   int16 i,j;
   for(i=x;i<=x+width;i++)
   {
      glcd_pixel(i,y,1);
      glcd_pixel(i,y+height,1);
   } 
   for(i=y;i<=y+height;i++)
   {
      glcd_pixel(x,i,1);
      glcd_pixel(x+width,i,1);
   }
   for(j=1;j<=depth;j++)
   {
      for(i=y+j;i<=y+height+j;i++)
      {
         glcd_pixel(x-j,i,1);
      }
   }
   for(j=1;j<=depth;j++)
   {
      for(i=x-j;i<=(x-j+width);i++)
      {
         glcd_pixel(i,y+height+j,color);
      }
   }
}


//****************************************************************************//
// Purpose:       Draw a bar (wide line) on a graphic LCD                     //
// Inputs:        (x1, y1) - the start coordinate                             //
//                (x2, y2) - the end coordinate                               //
//                width  - The number of pixels wide                          //
//                color - ON or OFF                                           //
// Dependencies:  glcd_pixel()                                                //
//****************************************************************************//
void glcd_bar(int x1, int y1, int x2, int y2, int width, int1 color)
{
   signed int  x, y, addx, addy, j;
   signed long P, dx, dy, c1, c2;
   int i;
   dx = abs((signed int)(x2 - x1));
   dy = abs((signed int)(y2 - y1));
   x = x1;
   y = y1;
   c1 = -dx*x1 - dy*y1;
   c2 = -dx*x2 - dy*y2;

   if(x1 > x2)
   {
      addx = -1;
      c1 = -dx*x2 - dy*y2;
      c2 = -dx*x1 - dy*y1;
   }
   else
      addx = 1;
   if(y1 > y2)
   {
      addy = -1;
      c1 = -dx*x2 - dy*y2;
      c2 = -dx*x1 - dy*y1;
   }
   else
      addy = 1;

   if(dx >= dy)
   {
      P = 2*dy - dx;

      for(i=0; i<=dx; ++i)
      {
         for(j=-(width/2); j<width/2+width%2; ++j)
         {
            if(dx*x+dy*(y+j)+c1 >= 0 && dx*x+dy*(y+j)+c2 <=0)
               glcd_pixel(x, y+j, color);
         }
         if(P < 0)
         {
            P += 2*dy;
            x += addx;
         }
         else
         {
            P += 2*dy - 2*dx;
            x += addx;
            y += addy;
         }
      }
   }
   else
   {
      P = 2*dx - dy;

      for(i=0; i<=dy; ++i)
      {
         if(P < 0)
         {
            P += 2*dx;
            y += addy;
         }
         else
         {
            P += 2*dx - 2*dy;
            x += addx;
            y += addy;
         }
         for(j=-(width/2); j<width/2+width%2; ++j)
         {
            if(dx*x+dy*(y+j)+c1 >= 0 && dx*x+dy*(y+j)+c2 <=0)
               glcd_pixel(x+j, y, color);
         }
      }
   }
}


//****************************************************************************//
// Purpose:       Draw a circle on a graphic LCD                              //
// Inputs:        (x,y) - the center of the circle                            //
//                radius - the radius of the circle                           //
//                fill - YES or NO                                            //
//                color - ON or OFF                                           //
// Dependencies:  glcd_pixel(), glcd_line()                                   //
//****************************************************************************//
void glcd_circle(int x, int y, int radius, int1 fill, int1 color)
{
   signed int a, b, P;
   a = 0;
   b = radius;
   P = 1 - radius;

   do
   {
      if(fill)
      {
         glcd_line(x-a, y+b, x+a, y+b, color);
         glcd_line(x-a, y-b, x+a, y-b, color);
         glcd_line(x-b, y+a, x+b, y+a, color);
         glcd_line(x-b, y-a, x+b, y-a, color);
      }
      else
      {
         glcd_pixel(a+x, b+y, color);
         glcd_pixel(b+x, a+y, color);
         glcd_pixel(x-a, b+y, color);
         glcd_pixel(x-b, a+y, color);
         glcd_pixel(b+x, y-a, color);
         glcd_pixel(a+x, y-b, color);
         glcd_pixel(x-a, y-b, color);
         glcd_pixel(x-b, y-a, color);
      }

      if(P < 0)
         P+= 3 + 2*a++;
      else
         P+= 5 + 2*(a++ - b--);
    } while(a <= b);
}


//****************************************************************************//
// Purpose:       Draw a segment of a circle on a graphic LCD                 //
// Inputs:        (x,y) - The center of the circle                            //
//                radius - The radius of the circle                           //
//                angle_start - Start point angle of segment of circle        //
//                angle_end - End point angle of segment of circle            //
//                                                                            //
// Example:       (angle_start = 0 & angle_end = 360 draws a normal circle)   //
//                (angle_start = 0 & angle_end = 180 draws half circle (arc) )//
//                                                                            //
//                color - ON or OFF                                           //
// Dependencies:  glcd_pixel(), <math.h>                                      //
//****************************************************************************//
void glcd_circle_seg(int x, int y, int radius, int16 angle_start, int16 angle_end, int1 color)
{
   int   x_round, y_round;
   float angle;
   #define DEGREE 2*3.14159265/360
   
   for (angle = (float)angle_start; angle <= angle_end; angle += .9)
   {
      x_round = (sin (angle * DEGREE) * radius);
      y_round = (cos (angle * DEGREE) * radius);
      glcd_pixel(x + x_round, y + y_round, color);
   }
}


//****************************************************************************//
// Purpose:       Write text on a graphic LCD                                 //
// Inputs:        (x,y) - The upper left coordinate of the first letter       //
//                textptr - A pointer to an array of text to display          //
//                size - The size of the text: 1 = 5x7, 2 = 10x14, ...        //
//                color - ON or OFF                                           //
// Dependencies:  glcd_pixel()                                                //
//****************************************************************************//
void glcd_text57(int x, int y, char* textptr, int size, int1 color)
{
   int i, j, k, l, m;                     // Loop counters
   BYTE pixelData[5];                     // Stores character data

   for(i=0; textptr[i] != '\0'; ++i, ++x) // Loop through the passed string
   {
      if(textptr[i] < 'S') // Checks if the letter is in the first text array
         memcpy(pixelData, TEXT[textptr[i]-' '], 5);
      else if(textptr[i] <= '~') // Check if the letter is in the second array
         memcpy(pixelData, TEXT2[textptr[i]-'S'], 5);
      else
         memcpy(pixelData, TEXT[0], 5);   // Default to space

      if(x+5*size >= GLCD_WIDTH)          // Performs character wrapping
      {
         x = 0;                           // Set x at far left position
         y += 7*size + 1;                 // Set y at next position down
      }
      for(j=0; j<5; ++j, x+=size)         // Loop through character byte data
      {
         for(k=0; k<7*size; ++k)          // Loop through the vertical pixels
         {
            if(bit_test(pixelData[j], k)) // Check if the pixel should be set
            {
               for(l=0; l<size; ++l)      // The next two loops change the
               {                          // character's size
                  for(m=0; m<size; ++m)
                  {
                     glcd_pixel(x+m, y+k*size+l, color); // Draws the pixel
                  }
               }
            }
         }
      }
   }
}


//****************************************************************************//
// Purpose:       Fill the LCD screen with the passed in color                //
// Inputs:        ON - turn all the pixels on                                 //
//                OFF - turn all the pixels off                               //
// Dependencies:  glcd_writeByte()                                            //
//****************************************************************************//
void glcd_fillScreen(int1 color)
{
   int i, j;

   // Loop through the vertical pages
   for(i = 0; i < 8; ++i)
   {
      output_low(GLCD_DI);                      // Set for instruction
      glcd_writeByte(GLCD_CS1, 0b01000000);     // Set horizontal address to 0
      glcd_writeByte(GLCD_CS2, 0b01000000);
      glcd_writeByte(GLCD_CS1, i | 0b10111000); // Set page address
      glcd_writeByte(GLCD_CS2, i | 0b10111000);
      output_high(GLCD_DI);                     // Set for data

      // Loop through the horizontal sections
      for(j = 0; j < 64; ++j)
      {
         glcd_writeByte(GLCD_CS1, 0xFF*color);  // Turn pixels on or off
         glcd_writeByte(GLCD_CS2, 0xFF*color);  // Turn pixels on or off
      }
   }
}


//****************************************************************************//
// Purpose:       Fill the graphic LCD screen with 128x64 bit converted image //
// Inputs:        YES - Inverse of image (Raw data saved in ROM)              //
//                NO - Normal image (Raw data saved in ROM)                   //
// Dependencies:  glcd_pixel(), glcd_init()                                   //
//                                                                            //
//                      !! Check the "Bitmap.h" file !!                       //
//                                                                            //
//****************************************************************************//
void glcd_image(int1 inverse)
{
   unsigned int i, j, k; 
   unsigned int16 count=0;
   int width = 128;  
   int height = 64;
   glcd_fillScreen(OFF);                        //Clears the screen (opt.)
   if(inverse)
   {
      for(i=0;i<height;i+=8)
      {
         for(j=0;j<width;j++)
         {
            for(k=0;k<8;k++)
            { 
               if(~bit_test(image[count],k))
               {
                  glcd_pixel(j,i+k,ON);
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
               if(bit_test(image[count],k))
               {
                  glcd_pixel(j,i+k,ON);
               }         
               
            }
            count++;
         }  
      }
   }
}


//****************************************************************************//
// Purpose:       Draw less or equal 128x64 pixels of image to the graphic LCD//
//                screen on specified x, y coordinate                         // 
// Inputs:        (x, y) - The start coordinate                               //
//                YES - Inverse of image (Raw data saved in ROM)              //
//                NO - Normal image (Raw data saved in ROM)                   //
// Dependencies:  glcd_pixel(), glcd_init()                                   //
//                                                                            //
//                       !!! PRLEASE BE CAREFULL !!                           //
//                                                                            //
//         First 2 bytes of your image data must be size of your BMP          //
//                                                                            //
// Example:       If your BMP image size is 64x32, first 2 bytes should       //
//                be 0x40, 0x20 --> (64 = 0x40, 32 = 0x20)                    //
//                                                                            //
//                      !! Check the "Bitmap.h" file !!                       //
//                                                                            //
//****************************************************************************//
void glcd_image_xy(int x, int y, int1 inverse)
{
   unsigned int i, j, k;
   unsigned int16 count=2;
   int width  = image_xy[0];
   int height = image_xy[1];
   glcd_fillScreen(OFF);                        //Clears the screen (opt.)
   if(inverse)
   {
      for(i=0;i<height;i+=8)
      {   
         for(j=0;j<width;j++)
         {
            for(k=0;k<8;k++)
            { 
               if(~bit_test(image_xy[count],k))
               {
                  glcd_pixel(x+j,y+i+k,ON);
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
               if(bit_test(image_xy[count],k))
               {
                  glcd_pixel(x+j,y+i+k,ON);
               }
            }
            count++;
         }
      }
   }
}
      
//****************************************************************************//
// Purpose:       Write a byte of data to the specified chip                  //
// Inputs:        chipSelect - which chip to write the data to                //
//                data - the byte of data to write                            //
//****************************************************************************//
void glcd_writeByte(char chip, BYTE data)
{
   if(chip == GLCD_CS1)       // Choose which chip to write to
      output_high(GLCD_CS1);
   else
      output_high(GLCD_CS2);

   output_low(GLCD_RW);       // Set for writing
   DATAOUT(data);            // Put the data on the port
   output_high(GLCD_E);       // Pulse the enable pin
   delay_us(2);
   output_low(GLCD_E);

   output_low(GLCD_CS1);      // Reset the chip select lines
   output_low(GLCD_CS2);
}

//****************************************************************************//
// Purpose:       Reads a byte of data from the specified chip                //
// Ouputs:        A byte of data read from the chip                           //
//****************************************************************************//
BYTE glcd_readByte(BYTE chip)
{
   BYTE data;                 // Stores the data read from the LCD
   if(chip == GLCD_CS1)       // Choose which chip to read from
      output_high(GLCD_CS1);
   else
      output_high(GLCD_CS2);

   DATAIN();                 // Set port d to input
   output_high(GLCD_RW);      // Set for reading
   output_high(GLCD_E);       // Pulse the enable pin
   delay_us(2);
   output_low(GLCD_E);
   delay_us(2);
   output_high(GLCD_E);       // Pulse the enable pin
   delay_us(2);
   data = DATAIN();          // Get the data from the display's output register
   output_low(GLCD_E);

   output_low(GLCD_CS1);      // Reset the chip select lines
   output_low(GLCD_CS2);
   return data;               // Return the read data
}

///////////////////////////////////////////////////






