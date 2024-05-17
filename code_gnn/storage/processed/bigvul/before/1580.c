aspath_filter_exclude (struct aspath * source, struct aspath * exclude_list)
{
  struct assegment * srcseg, * exclseg, * lastseg;
  struct aspath * newpath;

  newpath = aspath_new();
  lastseg = NULL;

  for (srcseg = source->segments; srcseg; srcseg = srcseg->next)
  {
    unsigned i, y, newlen = 0, done = 0, skip_as;
    struct assegment * newseg;

     
    for (i = 0; i < srcseg->length; i++)
    {
      skip_as = 0;
      for (exclseg = exclude_list->segments; exclseg && !skip_as; exclseg = exclseg->next)
        for (y = 0; y < exclseg->length; y++)
          if (srcseg->as[i] == exclseg->as[y])
          {
            skip_as = 1;
            break;
          }
      if (!skip_as)
        newlen++;
    }
     
    if (!newlen)
      continue;

     
    newseg = assegment_new (srcseg->type, newlen);
    for (i = 0; i < srcseg->length; i++)
    {
      skip_as = 0;
      for (exclseg = exclude_list->segments; exclseg && !skip_as; exclseg = exclseg->next)
        for (y = 0; y < exclseg->length; y++)
          if (srcseg->as[i] == exclseg->as[y])
          {
            skip_as = 1;
            break;
          }
      if (skip_as)
        continue;
      newseg->as[done++] = srcseg->as[i];
    }
     
    if (!lastseg)
      newpath->segments = newseg;
    else
      lastseg->next = newseg;
    lastseg = newseg;
  }
  aspath_str_update (newpath);
   
  aspath_free (source);
  return newpath;
}
