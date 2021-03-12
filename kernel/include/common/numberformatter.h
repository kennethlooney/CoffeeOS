#pragma once
#include <stdint.h>

namespace coffee
{
    namespace common
    {
        class NumberFormatter
        {
            public:
                const char* toString(uint64_t);
        };
    } // namespace common
    
} // namespace coffee
