#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "tftFonts.h"
#include "AD7843.h"
#include "tft.h"

void CreateButtons ();
char sBuffer[10] = { 0 };
char tBuffer[10] = { 0 };

volatile unsigned char RXdata, TXdata;
volatile int RXdata_Avail;
volatile unsigned char newClkTick = 0;
volatile long IntDegF;
volatile unsigned int tempReading[8] = { 759 };

volatile unsigned int xPos, yPos;
volatile unsigned char serviceTouch;
void *gpio_addr = NULL;
int mmapFD;

int
main ()
{
  int retval;
  unsigned long int iCount = 40;
  unsigned int yLine = 9;
  int xPos = 0;
  int yPos = 0;

  retval = MapGPIO ();
  if (retval < 0)
    {
      printf ("Error attempting to map GPIO1.\n");
    }
  TFT_Init ();
  TFT_Set_Address (0, 0, 239, 319);
  sleep(1); 
  TFT_Fill (GREEN);
  sleep(1); 
  TFT_Set_Address (0, 0, 239, 319);
  TFT_Fill (RED);
  sleep(1); 
  Write_Data (BLACK);
  TFT_Set_Address (0, 0, 239, 319);
  Write_Data (WHITE);
  for (xPos = 0; xPos < 240 - 32; xPos += 16)
    {
      TFT_Char ((char) iCount, xPos, yLine, 16, BLACK, WHITE);
      iCount++;
    }
   TFT_Fill (BLACK);

  TFT_Text ("This is a line.", 0, 250, 16, BLUE, BLACK);

  fclose(mmapFD); 
  return 0;



/* 	
	while(1)
	{
		
		if (serviceTouch == 1)
		{
			ReadTouch(&xPos, &yPos); 
			serviceTouch = 0; 
		}
		else
		{
			xPos = 0; 
			yPos = 0;
		}
		

		if (newClkTick) 
		{
			newClkTick = 0; 
		}

		//btn 0
		if( ((xPos > 10) && (xPos < 65) && (yPos > 2) && (yPos < 60)) )
		{
			hours += 1; 
			if ( !btnState0)
			{
				if (btnLastState0 == 0) CreateButton(0, 0, 78, 60, RED, PURPLE, "hit");
				btnState0 = 1; 
				btnLastState0 = 1; 
			}
		}
		else
		{
			if (btnState0) 
			{
				if (btnLastState0 == 1) CreateButton(0, 0, 78, 60, RED, GREEN, "Hour");
				btnState0 = 0; 
				btnLastState0 = 0; 
			}
		}

		//btn1
		if( ((xPos > 80) && (xPos < 155) && (yPos > 2) && (yPos < 60)) )
		{
			minutes += 1; 
			if ( !btnState1)
			{
				if (btnLastState1 == 0) CreateButton(79, 0, 156, 60, YELLOW, PURPLE, "hit");
				btnState1 = 1; 
				btnLastState1 = 1; 
			}
		}
		else
		{
			if (btnState1) 
			{
				if (btnLastState1 == 1) CreateButton(79 , 0, 156, 60, RED, WHITE , "Min");
				btnState1 = 0; 
				btnLastState1 = 0; 
			}
		}

		//btn2
		if( ((xPos > 160) && (xPos < 239) && (yPos > 2) && (yPos < 60)) )
		{
			if ( !btnState2)
			{
				if (btnLastState2 == 0) CreateButton(158, 0, 234, 60, YELLOW, PURPLE, "hit");
				btnState2 = 1; 
				btnLastState2 = 1; 
			}
		}
		else
		{
			if (btnState2) 
			{
				if (btnLastState2 == 1) CreateButton(158 , 0, 234, 60, RED, CYAN, "Test");
				btnState2 = 0; 
				btnLastState2 = 0; 
			}
		}
	}
}
*/
  
CreateButtons ()
{
  
  
  
  
  
  
  
  


/** 
* Create a button. Define top left and bottom right. 
* Border and Fill Color
* @param x1 
*/
  void
CreateButton (WORD x1, WORD y1, WORD x2, WORD y2, WORD border, WORD backcolor,
	      char *btnText)
{
  
  
  
  
    //Don't let x2 > 237
    if (x2 > 237)
    x2 = 237;
  TFT_Rectangle (x1, y1, x2, y2, border);
  count++;
  TFT_Rectangle (x1 + count, y1 + count, x2 + count, y2 - count, border);
  count++;
  TFT_Rectangle (x1 + count, y1 + count, x2 + count, y2 - count, border);
  TFT_Box (x1 + count, y1 + count, x2 - count, y2 - count, backcolor);
  count++;
  textX = x1 + ((x2 - x1) / 2) - (strlen (btnText) * 8);
  textY = y1 - 8 + (y2 - y1) / 2;
  TFT_Text (btnText, textX + count, textY, 16, BLACK, backcolor);
}

void
WriteCommandData (unsigned int Wcommand, unsigned int Wdata)
{
  SendWord (Wcommand);
  SendCommand (WRITECMD);
  SendWord (Wdata);
  SendCommand (WRITEDATA);
}

void


TFT_Init (void)
{

//***********************
//To do Reset TFT
//      P2OUT |= TFT_RESET;
  DelayMsec (5);

//      P2OUT &= ~TFT_RESET;
  DelayMsec (15);

//      P2OUT |= TFT_RESET;
  DelayMsec (15);
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  


Write_Command (unsigned int Wcommand)
{
  
  


Write_Data (unsigned int Wdata)
{
  
  


DelayMsec (unsigned int delay)
{
  


TFT_Set_Address (unsigned int PX1, unsigned int PY1, unsigned int PX2,
		 unsigned int PY2)
{
  
  WriteCommandData (0x0045, PY1);	// Vertical RAM address start
  WriteCommandData (0x0046, PY2);	// Vertical RAM address end
  WriteCommandData (0x004E, PX1);	// Set GDDRAM X address
  WriteCommandData (0x004F, PY1);	// Set GDDRAM Y address
  Write_Command (0x0022);


TFT_Fill (unsigned int color)
{
  
  
  
  

    {
      

	{
	  
	
    



Set_color (unsigned char r, unsigned char g, unsigned char b)
{
  
  
  
  
  



ZeroMemory (char *memory, unsigned int count)
{
  
  

    {
      
    



TFT_Text (char *S, WORD x, WORD y, BYTE DimFont, WORD Fcolor, WORD Bcolor)
{
  
  
  {
  0};
  
  
  

    {
      
      
      
    
  

    {
    
      

	{
	  
	  
	
      
    
      

	{
	  
	  
	
    



TFT_Text32 (char *S, WORD x, WORD y, WORD Fcolor, WORD Bcolor)
{
  
  
  {
  0};
  
  
  
  

    {
      
      
      
    
  

    {

      //Erase area in between characters
      TFT_Set_Address ((x + 32), y, (x + 35), y + 32);
      
      

	{

//                              P2OUT |= TFT_WR;
//                              P2OUT &= ~TFT_WR;
//                              P2OUT |= TFT_WR;
	}
      
      
    



TFT_32Char (char C1, unsigned int x, unsigned int y, unsigned int Fcolor,
	    unsigned int Bcolor)
{
  
  
  
  
  
  
  
  
  

    {
      
      

	{
	  

	    {
	      
	      
		

	      else

		{
		  
		
	    
	  
	
    



TFT_Char (char C1, unsigned int x, unsigned int y, unsigned char DimFont,
	  unsigned int Fcolor, unsigned int Bcolor)
{
  
  
  
  
  
  
  
  

    {
    

      {
	
	
	
	
	
	
	

	  {
	    
	    
	  
	

	  {
	    

	      {
		
		
		

		  {
		    
		    
		  

		else

		  {
		    
		    
		    
		  
		
	      
	    
	    
	    
	  
      
      
    

      {
	
	
	
	
	

	  {
	    
	    
	  
	

	  {
	    

	      {
		
		

		  {

		    //TFT_Dot(x_new,y_new,Fcolor);
		    Write_Data (Fcolor);
		    
		  

		else

		  {

		    //TFT_Dot(x_new,y_new,Bcolor);
		    Write_Data (Bcolor);
		    
		  
		
	      
	  
      
    



TFT_Box (unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,
	 unsigned int color)
{
  
  
  
  

    {
      

	{
        SendCommand(WRITEDATA); 
	}
    



TFT_Dot (unsigned int x, unsigned int y, unsigned int color)
{
  
  


TFT_Line (unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,
	  unsigned int color)
{
  
  
  
  
  
  
  
  

    {
      
    

  else

    {
      
      

	{
	  
	

      else

	{
	  
	
      

	{
	  
	  

	    {
	      
	      

		{
		  
		  
		

	      else

		{
		  
		  
		  
		
	    
	

      else

	{
	  
	  
	    
	
      

	{
	  
	  
	

      else

	{
	  
	  
	  
	
    



TFT_H_Line (unsigned char x1, unsigned char x2, unsigned char y_pos,
	    unsigned int color)
{
  
  

    {
      
    



TFT_V_Line (unsigned int y1, unsigned int y2, unsigned char x_pos,
	    unsigned int color)
{
  
  

    {
      
    



/* Writes an unfilled rectangle */
  void
TFT_Rectangle (unsigned int x1, unsigned int y1, unsigned int x2,
	       unsigned int y2, unsigned int color)
{
  
  
  
  


TFT_Circle (WORD x, WORD y, BYTE radius, BYTE fill, WORD color)
{
  
  
  
  
  

    {
      

	{
	  
	  
	  
	  
	

      else

	{
	  
	  
	  
	  
	  
	  
	  
	  
	
      

	{
	  
	  
	

      else

	{
	  
	  
	  
	
    



//Set Absolute GDRAM address. x Range is 0-319. Y Range is 0-239; 
//This sets the absolute address for writes. 
  void
Set_GDRAM_Address (int x1, int y1)
{
  
  WriteCommandData (0x004F, y1);	// Set GDDRAM Y address
  Write_Command (0x22);


TestLargeFont (void)
{
  
  
  
  
  

    {
      
      
      
      
      
      
      
      
      
      
      
    
  



/*
sub procedure TFT_Image(dim pos_x,pos_y,dim_x,dim_y as word,dim const picture as ^byte)
dim k,i,x,y,temp as word
TFT_CS  = 0
x =pos_x
y =pos_y

temp = picture^
picture = picture +1
temp = (temp << 8)
temp = temp + picture^
picture = picture +1

TFT_Set_Address(pos_x,pos_y,dim_x,dim_y)

for k = 0 to dim_y - 1
for i = 0 to dim_x - 1

TFT_Set_Address(x,y,x,y)
Write_Data(temp)
temp = picture^
picture = picture +1
temp = (temp << 8)
temp = temp + picture^
picture = picture +1
inc(x)
next i
x = 0
inc(y)
next k
TFT_CS  = 1
end sub
*/