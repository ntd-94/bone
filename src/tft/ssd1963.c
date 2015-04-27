/***************************************************************************************************************************************************************
This example code is 16bit mode for TFT4.3 / TFT5.0 demo.
 
The demo used TFT01 Mega Shield and connect the pins to Arduino first :
DB0-DB16 to pin 22-29, 30-37
 
All the data pins of Arduino is used ….So , we just can connect the RD to 3.3V ….We don’t used it because we don’t need to read back the data from the screen.
Connect the power pins : LCD-A to 3.3V , VCC to 5V , GND to GND
 
Note that if you haven't TFT01 Mega Shilld,the LCM is use the 3.3V voltage level , so if you want to connect the 5V Arduino pins to it , 
you need add a resister about 20K to reduce voltage. We use the 3.3V Arduino Pro which work in 8M , 
so we connect the pins directly — this is why we used 16 bit mode, 8 bit mode is too slow to refresh the screen.

Create by Robi.Wang  On 9-June-2012 
Version 2.2  www.elecfreaks.com
***************************************************************************************************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include "tft.h"

/* Touch IO  */
#define DCLK     6
#define CS       5  
#define DIN      4 
#define DOUT     3
#define IRQ      2 

unsigned int  HDP=479;
unsigned int  HT=531;
unsigned int  HPS=43;
unsigned int  LPS=8;
unsigned char HPW=10;


unsigned int  VDP=271;
unsigned int  VT=288;
unsigned int  VPS=12;
unsigned int  FPS=4;
unsigned char VPW=10;

struct pix_
{
	unsigned int x;
	unsigned int y;
};

struct pix_ Tp_pix;	

void userMode() {
    int i = 0, pacy = 1; 
    int ly,lx; 
                    lx=HDP-((Tp_pix.x-380)*10/69);
				if(lx>HDP) lx=HDP;
    				ly=VDP-((Tp_pix.y-600)*10/111);
                                    if(ly>VDP) ly=VDP;
    				Address_set(lx,ly,lx+2,ly+2);
                                switch(pacy)
                                {
                                  case 0: for(i=0; i<7; i++)  Lcd_Write_Data(0xF8,0x00);  break;   //Red
                                  case 1: for(i=0; i<7; i++)  Lcd_Write_Data(0xFF,0xE0);  break;   //Yellow
                                  case 2: for(i=0; i<7; i++)  Lcd_Write_Data(0xFF,0xFF);  break;   //White 
                                  case 3: for(i=0; i<7; i++)  Lcd_Write_Data(0x05,0x1F);  break;   //Blue
                                  case 4: for(i=0; i<7; i++)  Lcd_Write_Data(0x00,0x1F);  break;   //Blue-2
                                  case 5: for(i=0; i<7; i++)  Lcd_Write_Data(0xF8,0x1F);  break;   //Magenta
                                  case 6: for(i=0; i<7; i++)  Lcd_Write_Data(0x07,0xE0);  break;   //Green
                                  case 7: for(i=0; i<7; i++)  Lcd_Write_Data(0x7F,0xFF);  break;   //Cyan
                                  defoult:for(i=0; i<7; i++)  Lcd_Write_Data(0x00,0x00);  break;   //Black
                                }
}

 
void Lcd_Writ_Bus(char VH,char VL)
{
  SendWord((int)((VH<<8)&0xff00) + (int)VL);
  SendCommand(WRITEDATA); 
}
 
void Lcd_Write_Data(char VH,char VL)
{
  Write_Data((VH>>8) + VL);
}

void LCD_WR_Data(int da)
{
    Write_Data((da>>8) + (da & 0xff));
}	
 
void LCD_WR_REG(int da)	 
{	
    Write_Command((da>>8) + (da & 0xff));
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
    Write_Command(0x002A);		//Set Column Address	

    Write_Data(x1>>8);	    
    Write_Data(x1&0xff);
    Write_Data(x2>>8);
    Write_Data(x2&0xff);

    Write_Command(0x002b);		//Set page address

    Write_Data(y1>>8);	    
    Write_Data(y1&0x00ff);
    Write_Data(y2>>8);	    
    Write_Data(y2&0x00ff);

    Write_Command(0x002c);		//Write Memory Start
}
 
void Lcd_Init(void)
{
   SendDisplayReset();  
    Write_Command(0x00E2);	//PLL multiplier, set PLL clock to 120M
	Write_Data(0x002d);	    //N=0x36 for 6.5M, 0x23 for 10M crystal
	Write_Data(0x0002);
	Write_Data(0x0054);

	Write_Command(0x00E0);  // PLL enable
	Write_Data(0x0001);
	usleep(1000*5);       //Wait 5usec

	Write_Command(0x00E0);  //Use PLL as system clock. Enable PLL
	Write_Data(0x0003);
	usleep(1000*5);

	Write_Command(0x0001);  // software reset
	usleep(5000);
	Write_Command(0x00E6);	//PLL setting for PCLK, depends on resolution
	Write_Data(0x0000);
	Write_Data(0x00ff);
	Write_Data(0x00be);

	Write_Command(0x00B0);	//LCD SPECIFICATION
	Write_Data(0x0000);     //POR defaults 18 bit
	Write_Data(0x0000);
	Write_Data((HDP>>8)&0X00FF);  //Set HDP
	Write_Data(HDP&0X00FF);
    Write_Data((VDP>>8)&0X00FF);  //Set VDP
	Write_Data(VDP&0X00FF);
    Write_Data(0x0000);
	usleep(5*1000);

	Write_Command(0x00B4);	//HSYNC
	Write_Data((HT>>8)&0X00FF);  //Set HT
	Write_Data(HT&0X00FF);
	Write_Data((HPS>>8)&0X00FF);  //Set HPS
	Write_Data(HPS&0X00FF);
	Write_Data(HPW);			   //Set HPW
	Write_Data((LPS>>8)&0X00FF);  //SetLPS
	Write_Data(LPS&0X00FF);
	Write_Data(0x0000);

	Write_Command(0x00B6);	//VSYNC
	Write_Data((VT>>8)&0X00FF);   //Set VT
	Write_Data(VT&0X00FF);
	Write_Data((VPS>>8)&0X00FF);  //Set VPS
	Write_Data(VPS&0X00FF);
	Write_Data(VPW);			   //Set VPW
	Write_Data((FPS>>8)&0X00FF);  //Set FPS
	Write_Data(FPS&0X00FF);

	Write_Command(0x0029); //display on
    usleep(5000); 

/*
	LCD_WR_REG(0x00BE); //set PWM for B/L
	LCD_WR_Data(0x0006);
	LCD_WR_Data(0x00f0);
	LCD_WR_Data(0x0001);
	LCD_WR_Data(0x00f0);
	LCD_WR_Data(0x0000);
	LCD_WR_Data(0x0000);
*/
	Write_Command(0x0036); //rotation
	Write_Data(0x0000);

	Write_Command(0x00F0); //pixel data interface
	Write_Data(0x0003);
	usleep(5000);
/*
	Write_Command(0x00d0); 
	Write_Data(0x000d);

        //----------LCD RESET---GPIO0-------------------//
	Write_Command(0x00B8);
	Write_Data(0x0000);    //GPIO3=input, GPIO[2:0]=output
	Write_Data(0x0001);    //GPIO0 normal

	Write_Command(0x00BA);
	Write_Data(0x0000);  
*/
}
 

void Pant(char VH,char VL)
{
  int i,j;
  Address_set(0,0,HDP+1,VDP+1);
  for(i=0;i<HDP+1;i++)
  {
    for (j=0;j<VDP+1;j++)
    {
      Lcd_Write_Data(VH,VL);
    }
  }
}
 
unsigned char Makpix(struct pix_ pix1,struct pix_ pix2) //Filtering
{
    unsigned char l=0;
    int x=pix1.x>pix2.x?pix1.x-pix2.x:pix2.x-pix1.x;
    int y=pix1.y>pix2.y?pix1.y-pix2.y:pix2.y-pix1.y;
    if(x<10&&y<10)	
    {
    	l=1;
    	Tp_pix.x=pix1.x;
    	Tp_pix.y=pix1.y;
    }
    return l;
}


void setup()
{
  int p; 
 
  Lcd_Init();     
  Pant(0x00, 0x00);  
  usleep(500 * 1000);
}
 