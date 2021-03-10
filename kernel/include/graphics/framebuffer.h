#ifndef COFFEE_GRAPHICS_FRAMEBUFFER_H
#define COFFEE_GRAPHICS_FRAMEBUFFER_H
#include <stddef.h>
 
        typedef struct
        {
            void *BaseAddress;
            size_t BufferSize;
            unsigned int Width;
            unsigned int Height;
            unsigned int PixelsPerScanLine;

        } FRAMEBUFFER;

#endif
    
