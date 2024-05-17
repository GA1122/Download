fd_read_hunk (int fd, hunk_terminator_t terminator, long sizehint, long maxsize)
{
  long bufsize = sizehint;
  char *hunk = xmalloc (bufsize);
  int tail = 0;                  

  assert (!maxsize || maxsize >= bufsize);

  while (1)
    {
      const char *end;
      int pklen, rdlen, remain;

       

      pklen = fd_peek (fd, hunk + tail, bufsize - 1 - tail, -1);
      if (pklen < 0)
        {
          xfree (hunk);
          return NULL;
        }
      end = terminator (hunk, hunk + tail, pklen);
      if (end)
        {
           
          remain = end - (hunk + tail);
          assert (remain >= 0);
          if (remain == 0)
            {
               
              hunk[tail] = '\0';
              return hunk;
            }
          if (bufsize - 1 < tail + remain)
            {
              bufsize = tail + remain + 1;
              hunk = xrealloc (hunk, bufsize);
            }
        }
      else
         
        remain = pklen;

       

      rdlen = fd_read (fd, hunk + tail, remain, 0);
      if (rdlen < 0)
        {
          xfree (hunk);
          return NULL;
        }
      tail += rdlen;
      hunk[tail] = '\0';

      if (rdlen == 0)
        {
          if (tail == 0)
            {
               
              xfree (hunk);
              errno = 0;
              return NULL;
            }
          else
             
            return hunk;
        }
      if (end && rdlen == remain)
         
        return hunk;

       

      if (tail == bufsize - 1)
        {
           
          if (maxsize && bufsize >= maxsize)
            {
              xfree (hunk);
              errno = ENOMEM;
              return NULL;
            }
          bufsize <<= 1;
          if (maxsize && bufsize > maxsize)
            bufsize = maxsize;
          hunk = xrealloc (hunk, bufsize);
        }
    }
}
