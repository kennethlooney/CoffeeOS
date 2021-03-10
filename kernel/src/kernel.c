#include "graphics/framebuffer.h"
#include "graphics/simplefonts.h"

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
void _start(FRAMEBUFFER* newBuffer, PSF1_FONT* newFont)
{
    putChar(newBuffer, newFont, 0xffffffff, 'G', 10, 10);
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