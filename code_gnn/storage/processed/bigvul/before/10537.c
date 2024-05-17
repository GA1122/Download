fd_read_body (const char *downloaded_filename, int fd, FILE *out, wgint toread, wgint startpos,

              wgint *qtyread, wgint *qtywritten, double *elapsed, int flags,
              FILE *out2)
{
  int ret = 0;
#undef max
#define max(a,b) ((a) > (b) ? (a) : (b))
  int dlbufsize = max (BUFSIZ, 8 * 1024);
  char *dlbuf = xmalloc (dlbufsize);

  struct ptimer *timer = NULL;
  double last_successful_read_tm = 0;

   
  void *progress = NULL;

   
  bool progress_interactive = false;

  bool exact = !!(flags & rb_read_exactly);

   
  bool chunked = flags & rb_chunked_transfer_encoding;
  wgint skip = 0;

   
  wgint sum_read = 0;
  wgint sum_written = 0;
  wgint remaining_chunk_size = 0;

  if (flags & rb_skip_startpos)
    skip = startpos;

  if (opt.show_progress)
    {
      const char *filename_progress;
       
      wgint start = skip ? 0 : startpos;
      if (opt.dir_prefix)
        filename_progress = downloaded_filename + strlen (opt.dir_prefix) + 1;
      else
        filename_progress = downloaded_filename;
      progress = progress_create (filename_progress, start, start + toread);
      progress_interactive = progress_interactive_p (progress);
    }

  if (opt.limit_rate)
    limit_bandwidth_reset ();

   
  if (progress || opt.limit_rate || elapsed)
    {
      timer = ptimer_new ();
      last_successful_read_tm = 0;
    }

   
  if (opt.limit_rate && opt.limit_rate < dlbufsize)
    dlbufsize = opt.limit_rate;

   
  while (!exact || (sum_read < toread))
    {
      int rdsize;
      double tmout = opt.read_timeout;

      if (chunked)
        {
          if (remaining_chunk_size == 0)
            {
              char *line = fd_read_line (fd);
              char *endl;
              if (line == NULL)
                {
                  ret = -1;
                  break;
                }
              else if (out2 != NULL)
                fwrite (line, 1, strlen (line), out2);

              remaining_chunk_size = strtol (line, &endl, 16);
              xfree (line);

              if (remaining_chunk_size == 0)
                {
                  ret = 0;
                  line = fd_read_line (fd);
                  if (line == NULL)
                    ret = -1;
                  else
                    {
                      if (out2 != NULL)
                        fwrite (line, 1, strlen (line), out2);
                      xfree (line);
                    }
                  break;
                }
            }

          rdsize = MIN (remaining_chunk_size, dlbufsize);
        }
      else
        rdsize = exact ? MIN (toread - sum_read, dlbufsize) : dlbufsize;

      if (progress_interactive)
        {
           
          tmout = 0.95;
          if (opt.read_timeout)
            {
              double waittm;
              waittm = ptimer_read (timer) - last_successful_read_tm;
              if (waittm + tmout > opt.read_timeout)
                {
                   
                  tmout = opt.read_timeout - waittm;
                  if (tmout < 0)
                    {
                       
                      ret = -1, errno = ETIMEDOUT;
                      break;
                    }
                }
            }
        }
      ret = fd_read (fd, dlbuf, rdsize, tmout);

      if (progress_interactive && ret < 0 && errno == ETIMEDOUT)
        ret = 0;                 
      else if (ret <= 0)
        break;                   

      if (progress || opt.limit_rate || elapsed)
        {
          ptimer_measure (timer);
          if (ret > 0)
            last_successful_read_tm = ptimer_read (timer);
        }

      if (ret > 0)
        {
          int write_res;

          sum_read += ret;
          write_res = write_data (out, out2, dlbuf, ret, &skip, &sum_written);
          if (write_res < 0)
            {
              ret = (write_res == -3) ? -3 : -2;
              goto out;
            }
          if (chunked)
            {
              remaining_chunk_size -= ret;
              if (remaining_chunk_size == 0)
                {
                  char *line = fd_read_line (fd);
                  if (line == NULL)
                    {
                      ret = -1;
                      break;
                    }
                  else
                    {
                      if (out2 != NULL)
                        fwrite (line, 1, strlen (line), out2);
                      xfree (line);
                    }
                }
            }
        }

      if (opt.limit_rate)
        limit_bandwidth (ret, timer);

      if (progress)
        progress_update (progress, ret, ptimer_read (timer));
#ifdef WINDOWS
      if (toread > 0 && opt.show_progress)
        ws_percenttitle (100.0 *
                         (startpos + sum_read) / (startpos + toread));
#endif
    }
  if (ret < -1)
    ret = -1;

 out:
  if (progress)
    progress_finish (progress, ptimer_read (timer));

  if (elapsed)
    *elapsed = ptimer_read (timer);
  if (timer)
    ptimer_destroy (timer);

  if (qtyread)
    *qtyread += sum_read;
  if (qtywritten)
    *qtywritten += sum_written;

  xfree (dlbuf);

  return ret;
}
