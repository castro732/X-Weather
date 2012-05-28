/************************************************************************
*                                                                       *
* Module:            dht22.c                                           *
* Description:       dht22/11-specific code.                              *
*                                                                       *
*                                                                       *
* Date:            Authors:               Comments:                     *
* 18 Mar 2012      Jose Morales            Created                      *
*                                                                       *
*                                                                       *
************************************************************************/


/************************************************************************
*  Defines  *****  Definiciones                                         *
************************************************************************/
#define dht22 PIN_A3



/************************************************************************
*  Globar Vars and Constants ***** Variables y Constates globales       *
************************************************************************/
char dht_dat[5]; 


/************************************************************************
*  Function Prototypes                                                  *
************************************************************************/
void iniciar_dht();
void leer_dht(int16 *hum, int16 *temp);
char leer_dht_dat();



/************************************************************************
*                                                                       *
*  Purpose:    Init DHT22/11                                         *
*                                                                       *
*  Proposito:  Iniciar DHT22/11                                         *
*                                                                       *
*  Passed:     None                                                     *
*  Argumento:  None                                                     *
*  Returned:   None                                                     *
*  Retorno:    None                                                     *
*  Note:       None                                                     *
*                                                                       *
*                                                                       *
************************************************************************/
void iniciar_dht()
{    
  OUTPUT_HIGH(dht22);
}

/************************************************************************
*                                                                       *
*  Purpose:    Read DHT22/11                                            *
*                                                                       *
*  Proposito:  Leer DHT22/11                                            *
*                                                                       *
*  Passed:     &hum - int16 ** &temp - int16                            *
*  Argumento:  &hum - int16 ** &temp - int16                            *
*  Returned:   None                                                     *
*  Retorno:    None                                                     *
*  Note:       hum and temp are pointers                                *
*                                                                       *
*                                                                       *
************************************************************************/
void leer_dht(int16 *hum, int16 *temp)
{
  int dht_in;
  int li=0;

  int16 dht_check_sum;
  int16 tempe, humi;

  OUTPUT_LOW(dht22);
  delay_ms(10);
  OUTPUT_HIGH(dht22);
  //delay_ms(18);// retardo indicado por el fabricante
  //OUTPUT_HIGH(dht22);
  delay_us(30);// entre 22 y 28 us 
  dht_in=input(dht22);
  if(dht_in)
  {   
    /*Start condition 1*/ 
    //printf("\r\nStart condition 1 not met");
    return;
  }
  delay_us(80);
  dht_in=input(dht22);
  if(!dht_in) 
  {    
    /*Start condition 2*/ 
    //printf("\r\nStart condition 2 not met");
    return;
  }
  delay_us(80);
  for (li=0; li<5; li++)
  {
    dht_dat[li] = leer_dht_dat(); 
  }

  delay_us(10);
  OUTPUT_HIGH(dht22);

  dht_check_sum = dht_dat[0]+dht_dat[1]+dht_dat[2]+dht_dat[3]; 
  if(dht_dat[4]!=dht_check_sum)
  {   
    /*Checksum failed*/ 
    //printf("dht22 checksum error");
    return;
  }

  humi = make16(dht_dat[0],dht_dat[1]);
  tempe = make16(dht_dat[2],dht_dat[3]);
  *hum = humi;
  *temp = tempe;
  /*
  printf("Current humdity = ");
  printf("%lu",humedad);
  printf(" RH  ");

  printf("Temperature = ");
  printf("%lu",temperatura); // por ajustar segun caracteristicas +- 2ºC
  
  printf("C \n ");
  delay_ms(2000);
  */


}

/************************************************************************
*                                                                       *
*  Purpose:    Read data from DHT22/11                                  *
*                                                                       *
*  Proposito:  Leer data desde DHT22/11                                 *
*                                                                       *
*  Passed:     None                                                     *
*  Argumento:  None                                                     *
*  Returned:   result - int                                             *
*  Retorno:    result - int                                             *
*  Note:                                                                *
*                                                                       *
*                                                                       *
************************************************************************/
int leer_dht_dat()
{
  int h = 0;
  char result=0;
  for (h=0; h< 8; h++) 
  {
    while(input(dht22)==0);
    delay_us(40);
    if (input(dht22)==1)
    {
      result |=(1<<(7-h));
    }
    while (input(dht22)==1);
    
  }
  
  return result;
}
