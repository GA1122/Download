static inline void utf32_codepoint_to_utf8(uint8_t* dstP, char32_t srcChar, size_t bytes)
{
    dstP += bytes;
 switch (bytes)
 {  
 case 4: *--dstP = (uint8_t)((srcChar | kByteMark) & kByteMask); srcChar >>= 6;
 case 3: *--dstP = (uint8_t)((srcChar | kByteMark) & kByteMask); srcChar >>= 6;
 case 2: *--dstP = (uint8_t)((srcChar | kByteMark) & kByteMask); srcChar >>= 6;
 case 1: *--dstP = (uint8_t)(srcChar | kFirstByteMark[bytes]);
 }
}
