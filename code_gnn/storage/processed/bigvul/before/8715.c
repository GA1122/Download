url_has_scheme (const char *url)
{
  const char *p = url;

   
  if (!*p || !SCHEME_CHAR (*p))
    return false;
  ++p;
   
  while (*p && SCHEME_CHAR (*p))
    ++p;
   
  return *p == ':';
}
