find_header(uschar *name, BOOL exists_only, int *newsize, BOOL want_raw,
  uschar *charset)
{
BOOL found = name == NULL;
int comma = 0;
int len = found? 0 : Ustrlen(name);
int i;
uschar *yield = NULL;
uschar *ptr = NULL;

 

for (i = 0; i < 2; i++)
  {
  int size = 0;
  header_line *h;

  for (h = header_list; size < header_insert_maxlen && h != NULL; h = h->next)
    {
    if (h->type != htype_old && h->text != NULL)   
      {
      if (name == NULL || (len <= h->slen && strncmpic(name, h->text, len) == 0))
        {
        int ilen;
        uschar *t;

        if (exists_only) return US"1";       
        found = TRUE;
        t = h->text + len;                   
        if (!want_raw)                       
          while (isspace(*t)) t++;           
        ilen = h->slen - (t - h->text);      

         

        if (!want_raw)
          while (ilen > 0 && isspace(t[ilen-1])) ilen--;

         

        if (!want_raw && name != NULL && comma == 0 &&
            Ustrchr("BCFRST", h->type) != NULL)
          comma = 1;

         

        size += ilen + comma + 1;   

         

        if (i != 0)
          {
          if (size > header_insert_maxlen)
            {
            ilen -= size - header_insert_maxlen - 1;
            comma = 0;
            }
          Ustrncpy(ptr, t, ilen);
          ptr += ilen;

           

          if (!want_raw && ilen > 0)
            {
            if (comma != 0) *ptr++ = ',';
            *ptr++ = '\n';
            }
          }
        }
      }
    }

   

  if (i == 0)
    {
    if (!found) return NULL;
    if (size > header_insert_maxlen) size = header_insert_maxlen;
    *newsize = size + 1;
    ptr = yield = store_get(*newsize);
    }
  }

 

if (want_raw)
  {
  *ptr = 0;
  }

 

else
  {
  uschar *decoded, *error;
  if (ptr > yield && ptr[-1] == '\n') ptr--;
  if (ptr > yield && comma != 0 && ptr[-1] == ',') ptr--;
  *ptr = 0;
  decoded = rfc2047_decode2(yield, check_rfc2047_length, charset, '?', NULL,
    newsize, &error);
  if (error != NULL)
    {
    DEBUG(D_any) debug_printf("*** error in RFC 2047 decoding: %s\n"
      "    input was: %s\n", error, yield);
    }
  if (decoded != NULL) yield = decoded;
  }

return yield;
}
