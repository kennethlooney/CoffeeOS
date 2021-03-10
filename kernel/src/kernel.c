#include "graphics/framebuffer.h"
#include "graphics/simplefonts.h"
#include "graphics/point.h"
#include <stdint.h>
 
void putChar(FRAMEBUFFER* newBuffer, PSF1_FONT* newFont, unsigned int color, char chr, unsigned int xOffset, unsigned int yOffset)
{
	unsigned int* pixPtr = (unsigned int*)newBuffer->BaseAddress;
	char* fontPtr = newFont->glyphBuffer + (chr * newFont->psf1_Header->charsize);
	for(unsigned long y = yOffset; y < yOffset + 18; y++) {
		for(unsigned long x = xOffset; x < xOffset+8; x++)
		{
			if((*fontPtr & (0b10000000 >> (x  - xOffset))) > 0) {
				*(unsigned int*)(pixPtr + x + (y * newBuffer->PixelsPerScanLine)) = color;
			}
		}
		fontPtr++;
	}
}
Point CursorPosition;

void Print(FRAMEBUFFER* newBuffer, PSF1_FONT* newFont, unsigned int color, char* str)
{
	 

	char* chr = str;
	while(*chr != 0) {
		putChar(newBuffer, newFont, color, *chr, CursorPosition.X, CursorPosition.Y);
		CursorPosition.X+=8;
		if(CursorPosition.X + 8 > newBuffer->Width)
		{
			CursorPosition.X = 0;
			CursorPosition.Y += 18;
		}
		chr++;
	}
}
void _start(FRAMEBUFFER* newBuffer, PSF1_FONT* newFont)
{
    CursorPosition.X = 50;
	CursorPosition.Y = 120;
	for(int t = 0; t < 50; t+=1){
		Print(newBuffer,newFont, 0xffffffff, " Hello Kernel");
	}
	// unsigned int y = 50;
	// unsigned int bbp = 4;

	// for(unsigned int x = 0; x < newBuffer->Width / 2* bbp; x+=bbp)
	// {
	// 	*(unsigned int*)(x + (y * newBuffer->PixelsPerScanLine * bbp) + (void*)newBuffer->BaseAddress) = 0xffffffff;
	// }
	
	int a = 10;
    int b = (a * 100) - 20;
    while(1){}
}