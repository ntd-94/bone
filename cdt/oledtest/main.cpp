#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include "../oled1309/oled1309.h"
#include "../oled1309/gfxfont.h"
#include "../oled1309/glcdfont.h"
#include "../oled1309/oled1309.h"
#include "FreeMono24pt7b.h"
#include "FreeMono12pt7b.h"
#include "Fonts/FreeSerif12pt7b.h"
#include "Fonts/FreeSans12pt7b.h"

const unsigned char pic[]=
{/*--ER-OLED015-2.bmp  --*/
/*--  128x64  --*/
0xFF,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x21,0xFD,0x21,0xE1,0x01,
0xF9,0x49,0x49,0x49,0xF9,0x01,0x01,0x01,0x01,0xFD,0x45,0x45,0x45,0x45,0x45,0x45,
0xFD,0x01,0x01,0x09,0x89,0xC9,0xA9,0x99,0xED,0x89,0x89,0x89,0x89,0x09,0x01,0x11,
0x11,0x11,0x11,0xF1,0x55,0x59,0x51,0xD1,0x19,0x11,0x01,0x29,0xA9,0xF9,0xA5,0x25,
0x01,0x49,0x91,0x01,0xFD,0x01,0x01,0x11,0x11,0xFD,0x91,0x01,0x91,0x91,0xFD,0x91,
0x91,0x11,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x07,0x00,0x1F,0x10,
0x17,0x12,0x12,0x12,0x17,0x18,0x00,0x00,0x00,0x1F,0x08,0x08,0x08,0x08,0x08,0x08,
0x1F,0x00,0x00,0x08,0x08,0x04,0x02,0x10,0x1F,0x00,0x01,0x02,0x04,0x08,0x00,0x10,
0x10,0x08,0x06,0x01,0x10,0x10,0x10,0x0F,0x00,0x00,0x00,0x06,0x01,0x1F,0x00,0x01,
0x02,0x02,0x02,0x02,0x1F,0x01,0x00,0x11,0x11,0x1F,0x00,0x10,0x11,0x0A,0x04,0x0A,
0x11,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
0xFF,0x00,0x00,0x00,0xFE,0x92,0x92,0x92,0x92,0x80,0x70,0x2C,0x22,0x2C,0x70,0x80,
0x4C,0x92,0x92,0x64,0x00,0x02,0x02,0xFE,0x02,0x02,0x00,0xFE,0x12,0x12,0x32,0xCC,
0x00,0x00,0xFE,0x00,0x00,0x4C,0x92,0x92,0x64,0x00,0x00,0xFE,0x00,0x00,0xFE,0x0C,
0x10,0x60,0xFE,0x00,0x00,0x7C,0x82,0x82,0x92,0x74,0x00,0x00,0x00,0x00,0x02,0x02,
0xFE,0x02,0x02,0x00,0xFE,0x92,0x92,0x92,0x92,0x00,0x7C,0x82,0x82,0x82,0x44,0x00,
0x00,0xFE,0x10,0x10,0x10,0xFE,0x00,0x00,0x80,0x00,0x00,0x7C,0x82,0x82,0x82,0x44,
0x00,0x00,0x7C,0x82,0x82,0x82,0x7C,0x00,0x00,0x80,0x00,0x00,0x80,0x00,0x00,0xFE,
0x80,0x80,0x80,0x02,0x02,0xFE,0x02,0x02,0x00,0xFE,0x82,0x82,0x82,0x7C,0x00,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xC0,0x20,0x00,0x10,0x08,0xC8,0xC8,0x48,0x48,0x48,0x08,0xC8,
0xC8,0x48,0x48,0xC8,0x88,0x08,0x08,0x08,0x08,0x08,0x08,0x88,0xC8,0x48,0xC8,0x88,
0x08,0x08,0xC8,0xC8,0x08,0x08,0x08,0x08,0x08,0xC8,0xC8,0x48,0x48,0x48,0x08,0xC8,
0xC8,0x48,0x48,0xC8,0x88,0x08,0x88,0xC8,0x48,0xC8,0x88,0x08,0x08,0x88,0xC8,0xC8,
0x08,0x08,0x08,0xC8,0xC8,0x48,0x48,0x08,0x08,0x08,0x08,0x08,0x88,0xC8,0x48,0xC8,
0x88,0x08,0x10,0x20,0x20,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x0F,0x30,0x40,0x00,0x80,0x00,0x3F,0x3F,0x22,0x22,0x22,0x00,0x3F,
0x3F,0x04,0x0C,0x1F,0x33,0x20,0x08,0x08,0x08,0x00,0x0F,0x1F,0x30,0x20,0x30,0x1F,
0x0F,0x00,0x3F,0x3F,0x20,0x20,0x20,0x20,0x00,0x3F,0x3F,0x22,0x22,0x22,0x00,0x3F,
0x3F,0x20,0x20,0x3F,0x1F,0x00,0x1F,0x3F,0x20,0x3F,0x1F,0x00,0x03,0x01,0x3F,0x3F,
0x00,0x00,0x17,0x33,0x22,0x3E,0x1C,0x00,0x08,0x08,0x08,0x00,0x30,0x38,0x2C,0x27,
0x23,0x00,0x80,0x40,0x40,0x30,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x7F,0x00,
0x00,0x00,0x40,0x00,0x2C,0x47,0x45,0x39,0x00,0x03,0x00,0x03,0x00,0x00,0x00,0xFF,
0xFF,0x00,0x00,0x40,0x20,0xF0,0x00,0x00,0x20,0x10,0x90,0x60,0x00,0x60,0x90,0x90,
0x60,0x00,0x00,0x30,0x40,0xC0,0x30,0x00,0xE1,0x91,0x91,0x21,0x01,0x01,0xC1,0x21,
0xF1,0x01,0x01,0x01,0x81,0x41,0x41,0x41,0xF1,0x01,0x81,0x41,0x41,0x41,0x81,0x41,
0xF1,0x41,0x01,0x81,0x41,0x41,0x41,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0xF1,0x91,0x91,0xF1,0x01,0x01,0xC1,0x31,0x01,0x61,0x91,0x91,
0x21,0x01,0x01,0x01,0x01,0x01,0xF1,0x01,0xC1,0x41,0x41,0x81,0x41,0xE1,0x41,0x01,
0x81,0x41,0x40,0x80,0x00,0xC0,0x40,0x40,0x40,0xE0,0x50,0x00,0x40,0x40,0x40,0xC0,
0x00,0x80,0x40,0x40,0x80,0x00,0x80,0x40,0x40,0x80,0x00,0x00,0x00,0x00,0x00,0xFF,
0xFF,0x00,0x00,0x80,0x80,0x07,0x00,0x80,0x06,0x05,0x84,0x84,0x00,0x83,0x84,0x04,
0x03,0x80,0x00,0x06,0x81,0x81,0x06,0x80,0x83,0x04,0x04,0x83,0x00,0x03,0x82,0x82,
0x07,0x02,0x00,0x00,0xE3,0xE4,0x84,0x84,0x87,0x00,0x03,0x84,0x84,0x04,0x03,0x80,
0x87,0x84,0x80,0x04,0x05,0x05,0x82,0x80,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,
0xE0,0xE0,0x00,0xA0,0xA7,0x00,0x00,0x80,0x80,0x86,0x81,0x00,0x00,0x82,0x84,0x84,
0x83,0x80,0x00,0x00,0xE0,0xE0,0x07,0x00,0x87,0x80,0x80,0x07,0x80,0x87,0x04,0x00,
0x03,0x85,0x85,0x05,0x00,0x07,0x00,0x80,0x80,0x87,0x00,0x00,0x06,0x85,0x85,0x87,
0x80,0x03,0x04,0x84,0x82,0x80,0x83,0x85,0x85,0x85,0x80,0x80,0x00,0x00,0x00,0xFF,
0xFF,0x80,0x80,0x83,0x9F,0x9C,0x8F,0x83,0x8F,0x9C,0x9F,0x83,0x80,0x83,0x9F,0x9C,
0x8F,0x83,0x8F,0x9C,0x9F,0x83,0x80,0x83,0x9F,0x9C,0x8F,0x83,0x8F,0x9C,0x9F,0x83,
0x80,0x98,0x98,0x80,0x9F,0x9F,0x90,0x90,0x9F,0x8F,0x80,0x8F,0x9F,0x90,0x90,0x9F,
0x9F,0xC0,0xC7,0xFF,0xB8,0x9F,0x87,0x80,0x84,0x84,0x84,0x80,0x8F,0x9F,0x90,0x90,
0x9F,0x9F,0x80,0x9F,0x9F,0x80,0x8B,0x9B,0x96,0x96,0x9D,0x8D,0x80,0xFF,0xFF,0x90,
0x90,0x9F,0x8F,0x80,0x9F,0x9F,0x80,0x8D,0x9E,0x92,0x9F,0x9F,0xC0,0xC7,0xFF,0xB8,
0x9F,0x87,0x80,0x98,0x98,0x80,0x8F,0x9F,0x90,0x99,0x89,0x80,0x8F,0x9F,0x90,0x90,
0x9F,0x8F,0x80,0x9F,0x9F,0x80,0x80,0x9F,0x9F,0x80,0x80,0x9F,0x9F,0x80,0x80,0xFF,
};


int main(int argc, char **argv) {

	printf("Started\n");
	oled1309 display(1);

	display.setFont(FreeMono12pt7b);
	wiringPiSetup();
	display.init_Hardware();
	display.initDisplay();
	display.setContrast(0xFF);

	int picSize = sizeof(pic);
	printf("Size %d\n", picSize);
	display.Display_Picture((unsigned char *) pic);

	sleep(2);
	/*
	 //   SendByte(COMMAND,SSD1309_INVERTDISPLAY);

	 test();
	 fontTest(gfxFont);
	 puts("Done");
	 */
	return 0;

}
