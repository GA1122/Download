static inline size_t utf32_codepoint_utf8_length(char32_t srcChar)
{
 if (srcChar < 0x00000080) {
 return 1;
 } else if (srcChar < 0x00000800) {
 return 2;
 } else if (srcChar < 0x00010000) {
 if ((srcChar < kUnicodeSurrogateStart) || (srcChar > kUnicodeSurrogateEnd)) {
 return 3;
 } else {
 return 0;
 }
 }
 else if (srcChar <= kUnicodeMaxCodepoint) {
 return 4;
 } else {
 return 0;
 }
}
