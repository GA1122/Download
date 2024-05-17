size_t strlen32(const char32_t *s)
{
 const char32_t *ss = s;
 while ( *ss )
    ss++;
 return ss-s;
}
