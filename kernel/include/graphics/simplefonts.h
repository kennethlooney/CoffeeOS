#ifndef COFFEE_GRAPHICS_SIMPLEFONTS_H
#define COFFEE_GRAPHICS_SIMPLEFONTS_H

typedef struct {
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charsize;
} PSF1_HEADER;
typedef struct {
	PSF1_HEADER* psf1_Header;
	void* glyphBuffer;
} PSF1_FONT;

#endif