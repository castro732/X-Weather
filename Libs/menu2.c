#define title 0
#define submenu 1

void title1();
void submenu1(void);
void submenu2(void);
void submenu3(void);
void submenu4(void);


typedef struct
{
	char tipo;
	char *menutext[20];
	char *item;
	/*char item2[20];
	char item3[20];
	char item4[20];*/
}MENU;

int mm = 5;

MENU mainmenu[] = 	{
							{title,"Configuracion",0},
							{date,"-Fecha y Hora",0},
							{submenu1,"-Temperatura   ",0},
							{submenu2,"-Presion",0},
							{submenu3,"-Velocidad",0},
					};

MENU submenu1[] = 	{
		{title,"-Temperatura",0},
		{action,"--C",0},
		{action,"--F",0},
		
		
};

void menu_submenu(char menu)
{
	switch (menu)
	{
		case 2:
		{
			
			break;
		}
	}
}


void menu_enter(int8 tb, int8 color)
{
   glcd_line(124,tb,124,tb+7,color);
   glcd_line(125,tb+1,125,tb+6,color);
   glcd_line(126,tb+2,126,tb+5,color);
   glcd_line(127,tb+3,127,tb+4,color);
   
}

void menu_drawmain(void)
{
   	glcd_fillScreen(OFF);
	glcd_text57(0, 0, mainmenu[0].menutext, 1, ON);
   	glcd_bar(0, 9, 127, 9, 2, ON); //div

   	for(i=1;i<mm;i++)
   	{
		glcd_text57(0, 14+((i-1)*9), mainmenu[i].menutext, 1, ON);
   		menu_enter(14+((i-1)*9),ON);
   	}
   	
   	glcd_update();
}

void menu_select(char item, char menu)
{

	

}











void readbuttons(void)
{
while (button==PRESSED)
       {
         lastup = up;
         lastdown = down;
         lastleft = left;
         lastright = right;
         lastenter = enter;
         up = input(PIN_UP);
         down = input(PIN_DOWN);
         left = input(PIN_LEFT);
         right = input(PIN_RIGHT);
         enter = input(PIN_ENTER);
         if ((up!=lastup)||(down!=lastdown)||(enter!=lastenter)||(lastleft!=left)||(lastright!=right))
         {
            button = PRESSED;
            set_timer1(0);
            tim1 = 0;   
            output_high(PIN_C3);
         }        
       }
}