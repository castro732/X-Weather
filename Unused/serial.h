#include <18F4680.h>

#FUSES NOWDT                    //No Watch Dog Timer

#FUSES MCLR                     //Master Clear pin enabled
//#FUSES STVREN                   //Stack full/underflow will cause reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES HS


#use delay(clock=8000000)
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,errors)
