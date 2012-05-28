#include <18F4680.h>

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES WDT128                   //Watch Dog Timer uses 1:128 Postscale
#FUSES INTRC                       //High speed Osc (> 4mhz for PCM/PCH) (>10mhz for PCD)
#FUSES FCMEN                    //Fail-safe clock monitor enabled
#FUSES IESO                     //Internal External Switch Over mode enabled
#FUSES NOPUT                    //No Power Up Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES BORV21                   //Brownout reset at 2.1V
//#FUSES PBADEN                   //PORTB pins are configured as analog input channels on RESET
#FUSES LPT1OSC                  //Timer1 configured for low-power operation
#FUSES MCLR                     //Master Clear pin enabled
#FUSES STVREN                   //Stack full/underflow will cause reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES BBSIZ1K                  //1K words Boot Block size
#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
#FUSES NODEBUG                  //No Debug mode for ICD
#FUSES NOPROTECT                //Code not protected from reading
#FUSES NOCPB                    //No Boot Block code protection
#FUSES NOCPD                    //No EE protection
#FUSES NOWRT                    //Program memory not write protected
#FUSES NOWRTC                   //configuration not registers write protected
#FUSES NOWRTB                   //Boot block not write protected
#FUSES NOWRTD                   //Data EEPROM not write protected
#FUSES NOEBTR                   //Memory not protected from table reads
#FUSES NOEBTRB                  //Boot block not protected from table reads

#use delay(crystal=8000000)
#use rs232(baud=115200,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,errors)
