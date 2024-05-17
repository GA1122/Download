int strncmp16(const char16_t *s1, const char16_t *s2, size_t n)
{
 char16_t ch;
 int d = 0;

 if (n == 0) {
 return 0;
 }

 do {
    d = (int)(ch = *s1++) - (int)*s2++;
 if ( d || !ch ) {
 break;
 }
 } while (--n);

 return d;
}
