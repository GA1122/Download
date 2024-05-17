size_t strnlen16(const char16_t *s, size_t maxlen)
{
 const char16_t *ss = s;

  
 while ((maxlen > 0) && *ss) {
    ss++;
    maxlen--;
 }
 return ss-s;
}
