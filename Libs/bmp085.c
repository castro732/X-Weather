/************************************************************************
*                                                                       *
* Module:            BMP085.c                                           *
* Description:       BMP085-specific code.                              *
*                                                                       *
*                                                                       *
* Date:            Authors:               Comments:                     *
* 18 Feb 2012      Jose Morales            Created                      *
*                                                                       *
*                                                                       *
************************************************************************/


/************************************************************************
*  Defines  *****  Definiciones                                         *
************************************************************************/
#define      BMP085_R   0xEF     // Write address
#define      BMP085_W   0xEE     // Read address
#define      OSS        0        // Oversampling Setting

/************************************************************************
*  Globar Vars and Constants ***** Variables y Constates globales       *
************************************************************************/
const unsigned char OSS_conversion_time[] = {5, 8, 14, 26};

signed int16 ac1;
signed int16 ac2;
signed int16 ac3;
signed int16 b1;
signed int16 b2;
signed int16 mb;
signed int16 mc;
signed int16 md;
unsigned int16 ac4;
unsigned int16 ac5;
unsigned int16 ac6;
int16 but,bup;
int8 shift; 


/************************************************************************
*  Function Prototypes                                                  *
************************************************************************/

void BMP085_Calibration(void);
long bmp085Read2Bytes(unsigned char address);
int32 bmp085Read3Bytes(unsigned char address);
long bmp085ReadTemp(void);
int32 bmp085ReadPressure(void);
void bmp085Convert(long *temperature, long *pressure,unsigned char readings);




/************************************************************************
*                                                                       *
*  Purpose:    Will read two (or 3) sequential 8-bit registers,         *
*              and Return a 16-bit value.                               *
*  Proposito:  Leer 2 registros de 8 bits secuenciales y regresar       *
*              un valor de 16bits.                                      *
*  Passed:     Address - Unsigned char                                  *
*  Argumento:  Direccion                                                *
*  Returned:   Reading - Long                                           *
*  Retorno:    Lectura                                                  *
*  Note:       Return value must be typecast to an signed short         *
*              if reading a signed value!                               *
*                                                                       *
************************************************************************/

long bmp085Read2Bytes(unsigned char address)
{              
   unsigned int16 msb, lsb;
   long data=0;
   
   delay_ms(10);
   
   i2c_start(); 
   i2c_write(BMP085_W);       //   Escribir la direccion del sensor 
   i2c_write(address);        //   Direccion de memoria
   i2c_start();               //   Reinicio
   i2c_write(BMP085_R);       //   Direccion para leer del sensor
   msb=i2c_read(0);           //   Leer la direccion
   delay_ms(10);
   i2c_stop();                //   Parar
   
   delay_ms(10);
   
   i2c_start(); 
   i2c_write(BMP085_W);       //   Write required to clock in address 
   i2c_write(address+1);      //   Mem address 
   i2c_start();               //   Restart
   i2c_write(BMP085_R);       //   Device address for reading
   lsb=i2c_read(0);           //   Read address
   delay_ms(10);
   i2c_stop();                //   Stop

   data = msb << 8;           
   data = data | lsb;

   return data;
}

int32 bmp085Read3Bytes(unsigned char address)
{              
   unsigned int16 msb,lsb,xlsb;
   int32 data=0;
   
   delay_ms(10);
   
   i2c_start(); 
   i2c_write(BMP085_W);       //   Escribir la direccion del sensor 
   i2c_write(address);        //   Direccion de memoria
   i2c_start();               //   Reinicio
   i2c_write(BMP085_R);       //   Direccion para leer del sensor
   msb=i2c_read(0);           //   Leer la direccion
   delay_ms(10);
   i2c_stop();                //   Parar
   
   

   delay_ms(10);
   
   i2c_start(); 
   i2c_write(BMP085_W);       //   Write required to clock in address 
   i2c_write(address+1);      //   Mem address 
   i2c_start();               //   Restart
   i2c_write(BMP085_R);       //   Device address for reading
   lsb=i2c_read(0);           //   Read address
   delay_ms(10);
   i2c_stop();                //   Stop

   delay_ms(10);
   
   i2c_start(); 
   i2c_write(BMP085_W);       //   Write required to clock in address 
   i2c_write(address+2);      //   Mem address 
   i2c_start();               //   Restart
   i2c_write(BMP085_R);       //   Device address for reading
   xlsb=i2c_read(0);           //   Read address
   delay_ms(10);
   i2c_stop();                //   Stop

   

   

   data = (msb << 8);
   data = (data | lsb);
   data = (data << 8);
   data = (data | xlsb);
   data = (data >> (8-OSS));


   return data;
}
/************************************************************************
*
* Purpose:        Calibrates the BMP085 pressure sensor.
* Passed:         Void
* Returned:       Void
* Note:
*
************************************************************************/
void BMP085_Calibration(void)
{
  /* ac1 = 6690;
   ac2 = -1227;
   ac3 = -14421;
   ac4 = 33898;
   ac5 = 24422;
   ac6 = 22886;
   b1 = 5498;
   b2 = 66;
   mb = -32768;
   mc = -11075;
   md = 2432;
*/
   ac1 = bmp085Read2Bytes(0xAA);
   ac2 = bmp085Read2Bytes(0xAC);
   ac3 = bmp085Read2Bytes(0xAE);
   ac4 = bmp085Read2Bytes(0xB0);
   ac5 = bmp085Read2Bytes(0xB2);
   ac6 = bmp085Read2Bytes(0xB4);
   b1 = bmp085Read2Bytes(0xB6);
   b2 = bmp085Read2Bytes(0xB8);
   mb = bmp085Read2Bytes(0xBA);
   mc = bmp085Read2Bytes(0xBC);
   md = bmp085Read2Bytes(0xBE);
/*
   printf("%Ld \n",ac1);
   printf("%Ld \n",ac2);
   printf("%Ld \n",ac3);
   printf("%Lu \n",ac4);
   printf("%Lu \n",ac5);
   printf("%Lu \n",ac6);
   printf("%Ld \n",b1);
   printf("%Ld \n",b2);
   printf("%Ld \n",mb);
   printf("%Ld \n",mc);
   printf("%Ld \n",md);
   */
}


/************************************************************************
*
* Purpose:      Will read the 16-bit temperature value of BMP085 sensor.
* Passed:      Void
* Returned:      Long
* Note:
* 
************************************************************************/
long bmp085ReadTemp(void)
{
   i2c_start();

   i2c_write(BMP085_W);       // Write 0xEE
   i2c_write(0xF4);           // Write register address
   i2c_write(0x2E);           // Write register data for temp

   i2c_stop();

   delay_ms(10);              // Max time is 4.5ms

   return bmp085Read2Bytes(0xF6);
}

/************************************************************************
*
* Purpose:      Will read the 16-bit pressure value from BMP085 sensor.
* Passed:      Void
* Returned:      Long
* Note:
*
************************************************************************/
int32 bmp085ReadPressure(void)
{
   i2c_start();

   i2c_write(BMP085_W);      // Write 0xEE
   i2c_write(0xF4);      // Write register address
   i2c_write(0x34 | (OSS << 6));      // Write register data for temp

   i2c_stop();

   delay_ms(OSS_conversion_time[OSS]);      // Max time is 4.5ms
   
   return bmp085Read3Bytes(0xF6);
    
}

/************************************************************************
*
* Purpose:      Will find callibrated pressure + temperature
* Passed:      Long *temperature, long *pressure, unsigned char readings
* Returned:      Void
* Note:
*
************************************************************************/

// The bit-shift for possibly negative numbers.
// Note that divide has a higher precedence than
// bit shift, so this is not precisely the same.

void bmp085Convert(float *temperature, float *pressure)
{
   int32 ut,up;
   //float32 b5, b6, x3, b3, p, b4, b7;
   float32 aux1,aux2,aux3,aux4;
  
   float32 c3,c4,c5,c6,bb1,mmc,mmd,x,x0,x1,x2,y,y0,y1,y2,p0,p1,p2,z,s,alpha,temp,pres;
   
   ut = bmp085ReadTemp();
   ut = bmp085ReadTemp();     
   up = bmp085ReadPressure();
   up = bmp085ReadPressure();
   
   //ut = 0x7B50;
   //up = 0x883F;
   //printf("%LX \n",ut);
   //printf("%LX \n",up);

   //but = ut;   //Debug var
     
/*NEW CALCS*/

   c3 = 160*pow(2,-15)*ac3;
   c4 = pow(10,-3)*pow(2,-15)*ac4;
   bb1 =  25600*pow(2,-30)*b1;

   //printf("%e \n",c3);
   //printf("%e \n",c4);
   //printf("%e \n",bb1);

   c5 = ac5*pow(2,-15)/160;
   c6 = ac6;
   mmc = (0.08)*mc;
   mmd = md/160;

   //printf("%e \n",c5);
   //printf("%e \n",c6);
   //printf("mmc\n");
   //printf("%e \n",mmc);
   //printf("%e \n",mmd);

   x0 = ac1;
   x1 = 160*pow(2,-13)*ac2;
   x2 = 25600*pow(2,-25)*b2;

   //printf("%e \n",x0);
   //printf("%e \n",x1);
   //printf("%e \n",x2);

   y0 = c4*32768;
   y1 = c4*c3;
   y2 = c4*bb1;

   //printf("%e \n",y0);
   //printf("%e \n",y1);
   //printf("%e \n",y2);

   p0 = 3783/1600;
   p1 = 1-7357*pow(2,-20);
   p2 = 3038*100*pow(2,-36);

   //printf("%e \n",p0);
   //printf("%e \n",p1);
   //printf("%e \n",p2);

   /*Calculamos temperatura*/
   alpha = c5*(ut-c6); 
   temp = alpha + (mmc/(alpha+mmd));


   /*Calculamos presion*/
   s = temp - 25;
   x = x2*s*s+x1*s+x0;
   y = y2*s*s+y1*s+y0;
   z = (up-x)/y;
   pres = p2*z*z+p1*z+p0;



   *temperature = temp;
   *pressure = pres;


/**/
   
   
/****************BEGIN*****CALC*****TEMP*********************/   

/*
   aux1 = ut-ac6;
   aux2 = aux1*ac5;
   x1 = aux2/32768;
   //x1 = ((ut - ac6) * ac5 / pow(2,15));
   
   
   aux3 = x1+md;
   aux4 = mc/aux3;
   x2 = aux4*2048;
   //x2 = (mc * (pow(2,11))) / (x1 + md);
   
   b5 = x1 + x2;
   
  
   
   *temperature = ((b5 + 8) / 16);

/****************END*****CALC*****TEMP***********************/   


/****************BEGIN*****CALC*****PRES*********************/   
/*

   up = up >> (8-OSS);
   
   //bup = up; //Debug var
   
   b6 = b5 - 4000;
   
   
   aux1 = b6*b6;
   aux2 = aux1/4096;   
   aux3 = aux2*b2;
   x1 = aux3/2048;   
   //x1 = (b2 * (b6 * b6 / pow(2,12))) / pow(2,11);
   
   aux1 = ac2*b6;
   x2 = aux1/2048;
   //x2 = ac2 * b6 / pow(2,11);
   
   x3 = x1 + x2;

   aux1 = ac1*4;
   aux2 = aux1+x3;
   shift = aux2;
   aux3 = shift << OSS;
   aux4 = aux3+2;
   b3 = aux4/4;
   //b3 = ((((ac1 * 4) + x3) << OSS) + 2) / 4;
   
   aux1 = ac3*b6;
   x1 = aux1/8192;
   //x1 = ac3 * b6 / pow(2,13);
   
   aux1 = b6*b6;
   aux2 = aux1/4096;   
   aux3 = aux2*b1;
   x2 = aux3/65536;   
   //x2 = (b1 * (b6 * b6 / pow(2,12))) / pow(2,16);
   
   aux1 = x1+x2+2;
   x3 = aux1/4;
   //x3 = ((x1 + x2) + 2) / pow(2,2);
   
   aux1 = x3+32768;
   aux2 = ac4*aux1;
   b4 = aux2/32768;
   //b4 = (ac4 * (unsigned long) (x3 + 32768)) / pow(2,15);
   
   aux1 = up-b3;
   aux2 = 50000>>OSS;
   b7 = aux1*aux2;
   //b7 = ((unsigned long) up - b3) * (50000 >> OSS);
   
   if (b7 < 0x80000000)
   {
      aux1 = b7*2;
      p = aux1/b4;
   }
   else
   {
      aux1 = b7/b4;
      p = aux1*2;
   }

   
   aux1 = p/256;
   x1 = aux1*aux1;
   //x1 = p / pow(2,8);
   //x1 *= x1;
   aux2 = x1*3038;
   x1 = aux2/65536;
   //x1 = (x1 * 3038) / pow(2,16);
   
   aux1 = -7357*p;
   x2 = aux1/65536;
   //x2 = (-7357 * p) / pow(2,16);
   
   aux1 = x1+x2+3791;
   aux2 = aux1/16;
   p = p+aux2;
   *pressure = p;
   
/****************END*****CALC*****PRES***********************/  
}








