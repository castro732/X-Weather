///////////////////////////////////////////////////////////////////////////
////                         BOOTLOADER.C                              ////
////                                                                   ////
////  This program is an example stand alone bootloader.               ////
////                                                                   ////
////  This program must be loaded into a target chip using a device    ////
////  programmer.  Afterwards this program may be used to load new     ////
////  versions of the application program.                             ////
////                                                                   ////
////  This bootloader is designed to detect pin B5 low on reset.  It   ////
////  will then use the RS232 link to download a new program.          ////
////  Otherwise the application program is started.                    ////
////                                                                   ////
////  Use an RS232 link and the SIOW.EXE program to load a new HEX     ////
////  file into the target chip.                                       ////
////                                                                   ////
////  This example will work with the PCM and PCH compilers.  The      ////
////  following conditional compilation lines are used to include a    ////
////  valid device for each compiler.  Change the device, clock and    ////
////  RS232 pins for your hardware if needed.                          ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2004 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS    ////
//// C compiler.  This source code may only be distributed to other    ////
//// licensed users of the CCS C compiler.  No other use,              ////
//// reproduction or distribution is permitted without written         ////
//// permission.  Derivative programs created using this software      ////
//// in object code form are not restricted in any way.                ////
///////////////////////////////////////////////////////////////////////////


#if defined(__PCH__)
#include <18F4680.h>
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(crystal=32000000)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)  // Jumpers: 8 to 11, 7 to 12
#endif


#define _bootloader


#include <bootloader.h>
#include <loader.c>


#if defined(__PCM__)
 #org LOADER_END+1,LOADER_END+10
#elif defined(__PCH__)
 #org LOADER_END+2,LOADER_END+20
#endif
void application(void) {
  while(TRUE);
}

#if defined(__PCH__)
#org 0x40,0x7F
#else
#org 0x20,0x3F
#endif
void main(void) {
   if(!input(PIN_E2))
   {
      load_program();
   }

   application();
}

#ORG default

#int_global
void isr(void) {
   jump_to_isr(LOADER_END+5*(getenv("BITS_PER_INSTRUCTION")/8));
}


