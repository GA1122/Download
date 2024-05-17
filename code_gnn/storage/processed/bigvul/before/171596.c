char16_t *strcpy16(char16_t *dst, const char16_t *src)
{
 char16_t *q = dst;
 const char16_t *p = src;
 char16_t ch;

 do {
 *q++ = ch = *p++;
 } while ( ch );

 return dst;
}
