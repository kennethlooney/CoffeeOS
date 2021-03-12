#ifndef COFFEE_GRAPHICS_SIMPLEFONTS_H
#define COFFEE_GRAPHICS_SIMPLEFONTS_H

struct PSF1_HEADER {
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charsize;
} ;
struct PSF1_FONT {
	PSF1_HEADER* psf1_Header;
	void* glyphBuffer;
} ;

#endif