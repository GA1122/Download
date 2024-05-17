path_simplify (enum url_scheme scheme, char *path)
{
  char *h = path;                
  char *t = path;                
  char *beg = path;
  char *end = strchr (path, '\0');

  while (h < end)
    {
       

      if (h[0] == '.' && (h[1] == '/' || h[1] == '\0'))
        {
           
          h += 2;
        }
      else if (h[0] == '.' && h[1] == '.' && (h[2] == '/' || h[2] == '\0'))
        {
           
          if (t > beg)
            {
               
              for (--t; t > beg && t[-1] != '/'; t--)
                ;
            }
          else if (scheme == SCHEME_FTP
#ifdef HAVE_SSL
              || scheme == SCHEME_FTPS
#endif
              )
            {
               
              beg = t + 3;
              goto regular;
            }
          h += 3;
        }
      else
        {
        regular:
           
          if (t == h)
            {
               
              while (h < end && *h != '/')
                t++, h++;
              if (h < end)
                t++, h++;
            }
          else
            {
               
              while (h < end && *h != '/')
                *t++ = *h++;
              if (h < end)
                *t++ = *h++;
            }
        }
    }

  if (t != h)
    *t = '\0';

  return t != h;
}
