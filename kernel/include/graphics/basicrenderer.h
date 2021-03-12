#pragma once
#include <common/math.h>
#include <graphics/framebuffer.h>
#include <graphics/simplefonts.h>
using namespace coffee::common;
namespace coffee
{
    namespace graphics
    {
  
        class BasicRenderer
        {
        public:
            BasicRenderer(Framebuffer *framebuffer, PSF1_FONT* newFont);
            ~BasicRenderer();
            void PutChar(char chr, unsigned int xOffset, unsigned int yOffset);
            void Print(const char* str);
            void Print(const char* str, int x, int y);
            void NewLine();
        protected:
            Framebuffer *m_buffer;
            PSF1_FONT* m_font;
            Point m_CursorPosition;
            unsigned int m_color; 
        };
    } // namespace graphics

} // namespace coffee
