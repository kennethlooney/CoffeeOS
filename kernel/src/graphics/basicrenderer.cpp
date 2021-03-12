#include <graphics/basicrenderer.h>
using namespace coffee::graphics;

BasicRenderer::BasicRenderer(Framebuffer *framebuffer, PSF1_FONT *newFont)
{
    m_buffer = framebuffer;
    m_font = newFont;
    m_color = 0xffffffff;
    m_CursorPosition = {0,0};
}
void BasicRenderer::PutChar(char chr, unsigned int xOffset, unsigned int yOffset)
{
    unsigned int *pixPtr = (unsigned int *)m_buffer->BaseAddress;
    char *fontPtr = (char *)m_font->glyphBuffer + (chr * m_font->psf1_Header->charsize);
    for (unsigned long y = yOffset; y < yOffset + 18; y++)
    {
        for (unsigned long x = xOffset; x < xOffset + 8; x++)
        {
            if ((*fontPtr & (0b10000000 >> (x - xOffset))) > 0)
            {
                *(unsigned int *)(pixPtr + x + (y * m_buffer->PixelsPerScanLine)) = m_color;
            }
        }
        fontPtr++;
    }
}

void BasicRenderer::Print(const char* str)
{
	 

	char* chr = (char*)str;
	while(*chr != 0) {
		PutChar(*chr, m_CursorPosition.X, m_CursorPosition.Y);
		m_CursorPosition.X+=8;
		if(m_CursorPosition.X + 8 > m_buffer->Width)
		{
			m_CursorPosition.X = 0;
			m_CursorPosition.Y += 18;
		}
		chr++;
	}
}
void BasicRenderer::Print(const char* str, int x, int y)
{
    m_CursorPosition.X = x;
    m_CursorPosition.Y = y;
    Print(str);
}
void BasicRenderer::NewLine()
{
    m_CursorPosition.X = 0;
    m_CursorPosition.Y += 18;
}