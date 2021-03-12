#pragma once

namespace coffee
{
    namespace graphics
    {
        enum ColorValueType
        {
            ARGB,
        };
        class Color
        {
            public:
            Color(unsigned int value, enum ColorValueType type);
            ~Color();
            private:
            unsigned int intValue;
        };
    } // namespace graphics
    
} // namespace coffee
