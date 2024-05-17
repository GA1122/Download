size_t strlen16(const char16_t *s)
{
 const char16_t *ss = s;
 while ( *ss )
    ss++;
 return ss-s;
}
