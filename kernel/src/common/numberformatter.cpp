#include <common/numberformatter.h>
using namespace coffee::common;

const char* NumberFormatter::toString(uint64_t value)
{
    char buffer[128];
    uint8_t size;
    uint64_t sizeTest = value;
    while(sizeTest / 10 > 0) {
        sizeTest /= 10;
        size++;
    }
    uint8_t index = 0;
    while(value / 10 > 0)
    {
        uint8_t remainder = value % 10;
        value /= 10;
        buffer[size - index] = remainder + '0';
        index++;
    }
    uint8_t remainder = value % 10;
    buffer[size - index] = remainder + '0';
    buffer[size - index + 1] = 0;
    return buffer; 
}