static inline uint32_t utf8_to_utf32_codepoint(const uint8_t *src, size_t length)
{
 uint32_t unicode;

 switch (length)
 {
 case 1:
 return src[0];
 case 2:
            unicode = src[0] & 0x1f;
            utf8_shift_and_mask(&unicode, src[1]);
 return unicode;
 case 3:
            unicode = src[0] & 0x0f;
            utf8_shift_and_mask(&unicode, src[1]);
            utf8_shift_and_mask(&unicode, src[2]);
 return unicode;
 case 4:
            unicode = src[0] & 0x07;
            utf8_shift_and_mask(&unicode, src[1]);
            utf8_shift_and_mask(&unicode, src[2]);
            utf8_shift_and_mask(&unicode, src[3]);
 return unicode;
 default:
 return 0xffff;
 }

}
