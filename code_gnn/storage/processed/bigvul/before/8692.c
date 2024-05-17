rewrite_shorthand_url (const char *url)
{
  const char *p;
  char *ret;

  if (url_scheme (url) != SCHEME_INVALID)
    return NULL;

   
  p = strpbrk (url, ":/");
  if (p == url)
    return NULL;

   
  if (p && p[0] == ':' && p[1] == '/' && p[2] == '/')
    return NULL;

  if (p && *p == ':')
    {
       
      int digits = strspn (p + 1, "0123456789");
      if (digits && (p[1 + digits] == '/' || p[1 + digits] == '\0'))
        goto http;

       
      if ((ret = aprintf ("ftp://%s", url)) != NULL)
        ret[6 + (p - url)] = '/';
    }
  else
    {
    http:
       
      ret = aprintf ("http://%s", url);
    }
  return ret;
}
