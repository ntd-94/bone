/*
 * DispDraw.cpp
 *
 *  Created on: 5 Mar 2017
 *      Author: root
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "DispDraw.h"
#include "tft1963.h"


DispDraw::DispDraw() {

	memset(fBuffer, 0, sizeof(fBuffer));
	ptr = new int;
	*ptr = 0;
}

DispDraw::DispDraw(const DispDraw &obj) {
	puts("DisplayDraw copy constructor.");
	ptr = new int;
	*ptr = *obj.ptr; // copy the value
}


DispDraw::~DispDraw() {

}

/**
 * Write one dot to frame buffer
 */
void DispDraw::drawPixel(int16_t x, int16_t y, uint16_t color) {
	if (y > YMAXPIXEL + 1) {
		printf("Y coordinate is out of range in drawDot\n");
		return;
	}
	fBuffer[y * XMAXPIXEL + x] = color;
}


/**
 * Update entire display from buffer
 */
void DispDraw::bufftoDisplay() {

	int i, j;
	unsigned short curValue = 0x0002;
	unsigned short nextValue = 0xFFFF;

	setAddress(0, 0, XMAXPIXEL, YMAXPIXEL);
	for (i = 0; i < YMAXPIXEL + 1; i++) {
		for (j = 0; j < XMAXPIXEL + 1; j++) {
			nextValue = fBuffer[i * XMAXPIXEL + j];
			if (curValue == nextValue) {
				SendCommand(WRITEDATA);
				curValue = fBuffer[i * XMAXPIXEL + j];
			} else {
				Write_Command(SSD1963_WRITE_MEMORY_CONTINUE);
				Write_Data(fBuffer[i * XMAXPIXEL + j]);
				curValue = fBuffer[i * XMAXPIXEL + j];
			}
		}
	}
}


void DispDraw::drawVertLine(unsigned int y1, unsigned int y2,
		unsigned int x_pos, unsigned int color) {

	unsigned int k;

	setAddress(x_pos, y1, x_pos, y2);
	Write_Data(color);
	for (k = y1; k <= y2; k++) {
		SendCommand(WRITEDATA);
	}
}


void DispDraw::drawHorzLine(int x1, int x2, int y_pos, unsigned int color) {
	int k;

	setAddress(x1, y_pos, x2, y_pos);
	Write_Data(color);

	for (k = x1; k <= x2; k++) {
		drawPixel(k, y_pos, color);
	}
}

