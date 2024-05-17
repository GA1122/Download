ksba_name_get_uri (ksba_name_t name, int idx)
{
  const char *s = ksba_name_enum (name, idx);
  int n;
  char *buf;

  if (!s || strncmp (s, "(3:uri", 6))
    return NULL;   
  s += 6;
  for (n=0; *s && *s != ':' && digitp (s); s++)
    n = n*10 + atoi_1 (s);
  if (!n || *s != ':')
    return NULL;  
  s++;
  buf = xtrymalloc (n+1);
  if (buf)
    {
      memcpy (buf, s, n);
      buf[n] = 0;
    }
  return buf;
}
