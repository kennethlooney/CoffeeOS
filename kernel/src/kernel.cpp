#include <common/cstr.h>
#include <graphics/basicrenderer.h>

#include <stdint.h>


using namespace coffee::graphics;
 


 
extern "C" void _start(Framebuffer* newBuffer, PSF1_FONT* newFont)
{
    BasicRenderer renderer(newBuffer,newFont);
	renderer.Print("Initializing..");
 

	 
    while(1){}
}
