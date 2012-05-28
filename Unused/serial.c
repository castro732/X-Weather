#if defined(__PCH__)
#include <serial.h>         // Headers
#endif
#include <bootloader.h> // Bootloader

int r=0;
#INT_RDA
void rda_isr(void)
{
   int t;
   
   t = getc();
   r = 1;
}

void main()
{
   
   enable_interrupts(INT_RDA);
      enable_interrupts(GLOBAL);
   while (1)
   {
      
      if(r==1)
         {
            output_toggle(PIN_C2);
            r = 0;
         }
   }
}
