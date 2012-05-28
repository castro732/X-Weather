         {if (auxi==0x54)         //Comparamos a T
                  {
                     type=1;
                     for(i=0;i<15;i++)
                     {                   
                        string[i]=0x00;                          
                     }
                     i = 0;
                  }
                  if (auxi==0x50)         //Comparamos a P
                  {
                     type=2;
                     for(i=0;i<15;i++)
                     {                   
                        string[i]=0x00;                          
                     }
                     i = 0;
                  }
                  if (auxi==0x48)         //Comparamos a H
                  {
                     type=3;
                     for(i=0;i<15;i++)
                     {                   
                        string[i]=0x00;                          
                     }
                     i = 0;
                  }
                   if (auxi==0x41)         //Comparamos a A
                  {
                     type=4;
                     for(i=0;i<15;i++)
                     {                   
                        string[i]=0x00;                          
                     }
                     i = 0;
                  }
         
         
         
                           if (type==1)      //Temperatura
                  {
                  
                     if ((auxi!=0x0D)&&(auxi!=0x54))
                     {
                        string[i] = auxi;
                        i++;
                     }
                     if (auxi==0x0D)
                     {
                        str_init(tprint);
                        tt = atol(string);  
                        sprintf(ttt,"%3.1w",tt);
                        strcat(tprint,ttt);
                        strcat(tprint,UniTemp);
                        draw_sect();
                        glcd_text_sec(1, 1, tprint, OFF);
                        glcd_update();
                        
                     }
                  }
                  if (type==2)      //Presion
                  {
                     if ((auxi!=0x0D)&&(auxi!=0x50))
                     {
                        string[i] = auxi;
                        i++;
                     }
                     if (auxi==0x0D)
                     {
                        str_init(pprint);
                        pp = atoi32(string);
                        sprintf(ppp,"%6.0w",pp);
                        strcat(pprint,ppp);
                        strcat(pprint,UniPres);  
                        draw_sect();
                        glcd_text_sec(1, 2, pprint, OFF);
                        glcd_update();
                     }
                  }
                  if(type==3)       //Humedad
                  {
                     if ((auxi!=0x0D)&&(auxi!=0x48))
                     {
                        string[i] = auxi;
                        i++;
                     }
                     if (auxi==0x0D)
                     {
                        str_init(hprint);
                        hh = atol(string);
                        sprintf(hhh,"%2.1w",hh);
                        strcat(hprint,hhh);
                          
                        draw_sect();
                        glcd_text_sec(1, 2, hprint, OFF);
                        glcd_update();
                     }
                  }}




                  {
                     int n=7;
                     while(1)
                     {
                        draw_sect();
                  
                        readbuttons();
                        if((right==PRESSED)&&(left==!PRESSED))
                        {
                           n++;
                           if(n>58)
                           {
                              n=58;
                           }
                        }
                        if((right==!PRESSED)&&(left==PRESSED))
                        {
                           n--;
                           if(n<5)
                           {
                              n=5;
                           }
                        }
                        glcd_rect(3, 3, 60, 10, NO, OFF);
                        glcd_rect(5, 5, n, 8, YES, OFF);
                  
                        glcd_update();
                        button=!PRESSED;
                        //delay_ms(5000);
                     }}