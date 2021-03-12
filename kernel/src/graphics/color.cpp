#include <graphics/color.h>
using namespace coffee::graphics;

Color::Color(unsigned int value, enum ColorValueType type)
{
    intValue = value;
}
Color::~Color(){}