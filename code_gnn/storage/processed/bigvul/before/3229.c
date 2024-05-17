write_data (FILE *out, FILE *out2, const char *buf, int bufsize,
            wgint *skip, wgint *written)
{
  if (out == NULL && out2 == NULL)
    return 1;
  if (*skip > bufsize)
    {
      *skip -= bufsize;
      return 1;
    }
  if (*skip)
    {
      buf += *skip;
      bufsize -= *skip;
      *skip = 0;
      if (bufsize == 0)
        return 1;
    }

  if (out != NULL)
    fwrite (buf, 1, bufsize, out);
  if (out2 != NULL)
    fwrite (buf, 1, bufsize, out2);
  *written += bufsize;

   

   
#ifndef __VMS
  if (out != NULL)
    fflush (out);
  if (out2 != NULL)
    fflush (out2);
#endif  
  if (out != NULL && ferror (out))
    return -1;
  else if (out2 != NULL && ferror (out2))
    return -2;
  else
    return 0;
}
