///////////////////////////////////////////////////////////////////////////
////                            24xx1025.C                             ////
////            Driver para memoria serial EEPROM de 1Mbit             ////
////                                                                   ////
//// CHIP PINs:    1        |       A0       |  Chip select            ////
//// =========     2        |       A1       |  Chip select            ////
////               3        |       A2       |  Conectar a Vcc         ////
////               4        |       Vss      |                         ////
////               5        |       Vcc      |                         ////
////               6        |       WP       |  Vcc -> write protected ////
////               7        |       SCL      |                         ////
////               8        |       SDA      |                         ////
////                                                                   ////
////   init_ext_eeprom();                                              ////
////                                                                   ////
////   write_ext_eeprom(a, d);    Escribe el byte d en la dirección a. ////
////                                                                   ////
////   d = read_ext_eeprom(a);    Lee el byte d desde la dirección a.  ////
////                                                                   ////
////   write_ext_eeprom_page(CS,address,len);                          ////
////                                                                   ////
////   read_ext_eeprom_page(CS,address,len);                           ////
////                                                                   ////
////   escribe/lee una página de len (1..128) bytes comenzando en la   ////
////   dirección address del bloque CS (0..7) desde/hacia el buffer    ////
////   page_data.                                                      ////
////                          CS= B,A1,A0                              ////
////         block select         |  |  |        chip select pins      ////
////         ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯      ////
////   El programa ppal. debe definir EEPROM_SDA y EEPROM_SCL          ////
////   de lo contrario se cargan los valores por defecto...            ////
///////////////////////////////////////////////////////////////////////////
////       This source code is free. by Menta (C) Copyright 2007       ////
///////////////////////////////////////////////////////////////////////////

#ifndef EEPROM_SDA
   #define EEPROM_SDA  PIN_C4
   #define EEPROM_SCL  PIN_C3
#endif

#use i2c(master, sda=EEPROM_SDA, scl=EEPROM_SCL, FAST)

#define EEPROM_ADDRESS long int
#define EEPROM_SIZE   65535

BYTE page_data[128];    // buffer donde almaceno los datos de 1 página

////////////////////////////////////////////////////////////////// INICIAR /////
void init_ext_eeprom()
{
   output_float(EEPROM_SCL);
   output_float(EEPROM_SDA);
}

///////////////////////////////////////////////////////////////////// LEER /////
BYTE read_ext_eeprom(int CS, long int address)
{
   BYTE data;
   int comando;
   comando=0xa0+CS*2;
   i2c_start();
   i2c_write(comando);
   i2c_write(address>>8);
   i2c_write(address);
   i2c_start();
   i2c_write(comando+1);
   data=i2c_read(0); // lee, pero no reconoce
   i2c_stop();
   return(data);
}

void read_ext_eeprom_page(int CS, long int address, unsigned int len)
{
   int comando,x;
   comando=0xa0+CS*2;
   i2c_start();
   i2c_write(comando);
   i2c_write(address>>8);
   i2c_write(address);
   i2c_start();
   i2c_write(comando+1);
   for(x=0;x<(len-1);x++) page_data[x]=i2c_read(); // lee y reconoce
   page_data[x]=i2c_read(0);   // lee, pero no reconoce //x=len-1
   i2c_stop();
}

///////////////////////////////////////////////////////////////// ESCRIBIR /////
void write_ext_eeprom(int CS, long int address, BYTE data)
{
   short int status;
   int comando;
   comando=0xa0+CS*2;
   i2c_start();
   i2c_write(comando);
   i2c_write(address>>8);
   i2c_write(address);
   i2c_write(data);
   i2c_stop();
   i2c_start();
   status=i2c_write(comando);
   while(status==1)
   {
      i2c_start();
      status=i2c_write(comando);
   }
}

void write_ext_eeprom_page(int CS, long int address, unsigned int len, char *page_data)
{
   short int status;
   int comando,x;
   BYTE data;
   comando=0xa0+CS*2;
   i2c_start();
   i2c_write(comando);
   i2c_write(address>>8);
   i2c_write(address);
   for(x=0;x<(len);x++)
   {
   data=page_data[x];
   i2c_write(data);
   }
   i2c_stop();
   i2c_start();
   status=i2c_write(comando);
   while(status==1)
   {
      i2c_start();
      status=i2c_write(comando);
   }

}

int1 write_check_ext_eeprom_page(unsigned int CS, long int address, unsigned int len)
{
   short int status,check=0;
   int comando,x;
   BYTE page_data_aux[128];
   BYTE data;
   comando=(0xa0)+(CS<<1);
   i2c_start();
   i2c_write(comando);
   i2c_write(address>>8);
   i2c_write(address);
   for(x=0;x<(len);x++)
   { data=page_data[x]; i2c_write(data); }
   i2c_stop();
   i2c_start();
   status=i2c_write(comando);
   while(status==1)
   {
      i2c_start();
      status=i2c_write(comando);
   }
   for(x=0;x<128;x++) page_data_aux[x]=page_data[x];  //duplico el array en RAM
   read_ext_eeprom_page(CS,address,len);
   for(x=0;x<128;x++)
   if (page_data_aux[x]!=page_data[x]) check=1;
   if (check) for(x=0;x<128;x++) page_data[x]=page_data_aux[x]; //recupero valor
   return(check);
}

////////////////////////////////////////////////////////////////////////////////



