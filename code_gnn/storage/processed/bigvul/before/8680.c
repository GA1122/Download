append_uri_pathel (const char *b, const char *e, bool escaped,
                   struct growable *dest)
{
  const char *p;
  int quoted, outlen;

  int mask;
  if (opt.restrict_files_os == restrict_unix)
    mask = filechr_not_unix;
  else if (opt.restrict_files_os == restrict_vms)
    mask = filechr_not_vms;
  else
    mask = filechr_not_windows;
  if (opt.restrict_files_ctrl)
    mask |= filechr_control;

   
  if (escaped)
    {
      char *unescaped;
      BOUNDED_TO_ALLOCA (b, e, unescaped);
      url_unescape (unescaped);
      b = unescaped;
      e = unescaped + strlen (unescaped);
    }

   
  if (e - b == 2 && b[0] == '.' && b[1] == '.')
    {
      b = "%2E%2E";
      e = b + 6;
    }

   
  quoted = 0;
  for (p = b; p < e; p++)
    if (FILE_CHAR_TEST (*p, mask))
      ++quoted;

   
  outlen = (e - b) + (2 * quoted);
  GROW (dest, outlen);

  if (!quoted)
    {
       
      memcpy (TAIL (dest), b, outlen);
    }
  else
    {
      char *q = TAIL (dest);
      for (p = b; p < e; p++)
        {
          if (!FILE_CHAR_TEST (*p, mask))
            *q++ = *p;
          else
            {
              unsigned char ch = *p;
              *q++ = '%';
              *q++ = XNUM_TO_DIGIT (ch >> 4);
              *q++ = XNUM_TO_DIGIT (ch & 0xf);
            }
        }
      assert (q - TAIL (dest) == outlen);
    }

   
  if (opt.restrict_files_case == restrict_lowercase
      || opt.restrict_files_case == restrict_uppercase)
    {
      char *q;
      for (q = TAIL (dest); q < TAIL (dest) + outlen; ++q)
        {
          if (opt.restrict_files_case == restrict_lowercase)
            *q = c_tolower (*q);
          else
            *q = c_toupper (*q);
        }
    }

  TAIL_INCR (dest, outlen);
  append_null (dest);
}
