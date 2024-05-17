uri_merge (const char *base, const char *link)
{
  int linklength;
  const char *end;
  char *merge;

  if (url_has_scheme (link))
    return xstrdup (link);

   
  end = path_end (base);
  linklength = strlen (link);

  if (!*link)
    {
       
      return xstrdup (base);
    }
  else if (*link == '?')
    {
       
       
       
       
       
      int baselength = end - base;
      merge = xmalloc (baselength + linklength + 1);
      memcpy (merge, base, baselength);
      memcpy (merge + baselength, link, linklength);
      merge[baselength + linklength] = '\0';
    }
  else if (*link == '#')
    {
       
       
       
       
      int baselength;
      const char *end1 = strchr (base, '#');
      if (!end1)
        end1 = base + strlen (base);
      baselength = end1 - base;
      merge = xmalloc (baselength + linklength + 1);
      memcpy (merge, base, baselength);
      memcpy (merge + baselength, link, linklength);
      merge[baselength + linklength] = '\0';
    }
  else if (*link == '/' && *(link + 1) == '/')
    {
       

       
       
       

      int span;
      const char *slash;
      const char *start_insert;

       
      slash = memchr (base, '/', end - base);
       
      if (slash && *(slash + 1) == '/')
        start_insert = slash;
      else
        start_insert = base;

      span = start_insert - base;
      merge = xmalloc (span + linklength + 1);
      if (span)
        memcpy (merge, base, span);
      memcpy (merge + span, link, linklength);
      merge[span + linklength] = '\0';
    }
  else if (*link == '/')
    {
       
      int span;
      const char *slash;
      const char *start_insert = NULL;  
      const char *pos = base;
      bool seen_slash_slash = false;
       
    again:
      slash = memchr (pos, '/', end - pos);
      if (slash && !seen_slash_slash)
        if (*(slash + 1) == '/')
          {
            pos = slash + 2;
            seen_slash_slash = true;
            goto again;
          }

       

      if (!slash && !seen_slash_slash)
         
         
        start_insert = base;
      else if (!slash && seen_slash_slash)
         
         
        start_insert = end;
      else if (slash && !seen_slash_slash)
         
         
        start_insert = base;
      else if (slash && seen_slash_slash)
         
         
        start_insert = slash;

      span = start_insert - base;
      merge = xmalloc (span + linklength + 1);
      if (span)
        memcpy (merge, base, span);
      memcpy (merge + span, link, linklength);
      merge[span + linklength] = '\0';
    }
  else
    {
       
      bool need_explicit_slash = false;
      int span;
      const char *start_insert;
      const char *last_slash = find_last_char (base, end, '/');
      if (!last_slash)
        {
           
          start_insert = base;
        }
      else if (last_slash && last_slash >= base + 2
               && last_slash[-2] == ':' && last_slash[-1] == '/')
        {
           
           
          start_insert = end + 1;
          need_explicit_slash = true;
        }
      else
        {
           
           
          start_insert = last_slash + 1;
        }

      span = start_insert - base;
      merge = xmalloc (span + linklength + 1);
      if (span)
        memcpy (merge, base, span);
      if (need_explicit_slash)
        merge[span - 1] = '/';
      memcpy (merge + span, link, linklength);
      merge[span + linklength] = '\0';
    }

  return merge;
}
