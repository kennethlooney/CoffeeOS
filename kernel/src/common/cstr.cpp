#include <common/cstr.h>

char outputStringu64[128];
const char* coffee::common::toString(uint64_t value)
{
   
   
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
        outputStringu64[size - index] = remainder + '0';
        index++;
    }
    uint8_t remainder = value % 10;
    outputStringu64[size - index] = remainder + '0';
    outputStringu64[size + 1] = 0;
    return outputStringu64; 
}

char outputStringi64[128];
const char* coffee::common::toString(int64_t value)
{
   uint8_t isNegative = 0;
   if(value < 0)
   {
       isNegative = 1;
       value *= -1;
       outputStringi64[0] = '-';
   }
   
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
        outputStringi64[isNegative + size - index] = remainder + '0';
        index++;
    }
    uint8_t remainder = value % 10;
    outputStringi64[isNegative + size - index] = remainder + '0';
    outputStringi64[isNegative + size + 1] = 0;
    return outputStringi64; 
}

char outputDoubleString[128];
const char* coffee::common::toString(double value, uint8_t places)
{
  if(places > 20) places = 20;

  char* intPtr = (char*)toString((int64_t)value);
  char* doublePtr = outputDoubleString;

  if(value < 0) {
      value *= -1;
     
  }
  while(*intPtr != 0)
  {
    *doublePtr = *intPtr;
    intPtr++;
    doublePtr++;
  }

  *doublePtr = '.';
  doublePtr++;

  double newValue = value - (int)value;
  for(uint8_t i = 0; i < places; i++)
  {
      newValue *= 10;
      *doublePtr = (int)newValue + '0';
      newValue -= (int)newValue;
      doublePtr++;
  }
  *doublePtr = 0;
  return outputDoubleString;
}

const char* coffee::common::toString(double value)
{
    return toString((double)value, 2);
}
char outputHex64String[128];
const char* coffee::common::toHexString(uint64_t value)
{
    uint64_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 8 * 2 - 1;
    for(uint8_t i = 0; i < size; i++)
    {
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        outputHex64String[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        outputHex64String[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }

    outputHex64String[size + 1] = 0;
    return outputHex64String;
}

char outputHex32String[128];
const char* coffee::common::toHexString(uint32_t value)
{
    uint32_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 4 * 2 - 1;
    for(uint8_t i = 0; i < size; i++)
    {
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        outputHex32String[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        outputHex32String[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }

    outputHex32String[size + 1] = 0;
    return outputHex32String;
}


char outputHex16String[128];
const char* coffee::common::toHexString(uint16_t value)
{
    uint16_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 2 * 2 - 1;
    for(uint8_t i = 0; i < size; i++)
    {
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        outputHex16String[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        outputHex16String[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }

    outputHex16String[size + 1] = 0;
    return outputHex16String;
}

char outputHex8String[128];
const char* coffee::common::toHexString(uint8_t value)
{
    uint8_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 1 * 2 - 1;
    for(uint8_t i = 0; i < size; i++)
    {
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        outputHex8String[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        outputHex8String[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }

    outputHex8String[size + 1] = 0;
    return outputHex8String;
}








 