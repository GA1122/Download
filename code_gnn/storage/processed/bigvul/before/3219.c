line_terminator (const char *start _GL_UNUSED, const char *peeked, int peeklen)
{
  const char *p = memchr (peeked, '\n', peeklen);
  if (p)
     
    return p + 1;
  return NULL;
}
