ftp_retrieve_glob (struct url *u, ccon *con, int action)
{
  struct fileinfo *f, *start;
  uerr_t res;

  con->cmd |= LEAVE_PENDING;

  res = ftp_get_listing (u, con, &start);
  if (res != RETROK)
    return res;
   
  if (opt.accepts || opt.rejects)
    {
      f = start;
      while (f)
        {
          if (f->type != FT_DIRECTORY && !acceptable (f->name))
            {
              logprintf (LOG_VERBOSE, _("Rejecting %s.\n"),
                         quote (f->name));
              f = delelement (f, &start);
            }
          else
            f = f->next;
        }
    }
   
  f = start;
  while (f)
    {
      if (has_insecure_name_p (f->name) || is_invalid_entry (f))
        {
          logprintf (LOG_VERBOSE, _("Rejecting %s.\n"),
                     quote (f->name));
          f = delelement (f, &start);
        }
      else
        f = f->next;
    }
   
  if (*u->file)
    {
      if (action == GLOB_GLOBALL)
        {
          int (*matcher) (const char *, const char *, int)
            = opt.ignore_case ? fnmatch_nocase : fnmatch;
          int matchres = 0;

          f = start;
          while (f)
            {
              matchres = matcher (u->file, f->name, 0);
              if (matchres == -1)
                {
                  logprintf (LOG_NOTQUIET, _("Error matching %s against %s: %s\n"),
                             u->file, quotearg_style (escape_quoting_style, f->name),
                             strerror (errno));
                  break;
                }
              if (matchres == FNM_NOMATCH)
                f = delelement (f, &start);  
              else
                f = f->next;         
            }
          if (matchres == -1)
            {
              freefileinfo (start);
              return RETRBADPATTERN;
            }
        }
      else if (action == GLOB_GETONE)
        {
#ifdef __VMS
           
          int (*cmp) (const char *, const char *)
            = opt.ignore_case ? strcasecmp : (int (*)())strcmp;
#else  
          int (*cmp) (const char *, const char *)
            = opt.ignore_case ? strcasecmp : strcmp;
#endif  
          f = start;
          while (f)
            {
              if (0 != cmp(u->file, f->name))
                f = delelement (f, &start);
              else
                f = f->next;
            }
        }
    }
  if (start)
    {
       
      res = ftp_retrieve_list (u, start, con);
    }
  else
    {
      if (action == GLOB_GLOBALL)
        {
           
           
          logprintf (LOG_VERBOSE, _("No matches on pattern %s.\n"),
                     quote (u->file));
        }
      else if (action == GLOB_GETONE)  
        {
           
          con->st |= ON_YOUR_OWN;
          res = ftp_loop_internal (u, NULL, con, NULL, false);
          return res;
        }

       
    }
  freefileinfo (start);
  if (opt.quota && total_downloaded_bytes > opt.quota)
    return QUOTEXC;
  else
    return res;
}
