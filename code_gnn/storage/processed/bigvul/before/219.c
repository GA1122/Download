ftp_loop (struct url *u, char **local_file, int *dt, struct url *proxy,
          bool recursive, bool glob)
{
  ccon con;                      
  uerr_t res;

  *dt = 0;

  xzero (con);

  con.csock = -1;
  con.st = ON_YOUR_OWN;
  con.rs = ST_UNIX;
  con.id = NULL;
  con.proxy = proxy;

   
  if (!*u->file && !recursive)
    {
      struct fileinfo *f;
      res = ftp_get_listing (u, &con, &f);

      if (res == RETROK)
        {
          if (opt.htmlify && !opt.spider)
            {
              char *filename = (opt.output_document
                                ? xstrdup (opt.output_document)
                                : (con.target ? xstrdup (con.target)
                                   : url_file_name (u, NULL)));
              res = ftp_index (filename, u, f);
              if (res == FTPOK && opt.verbose)
                {
                  if (!opt.output_document)
                    {
                      struct_stat st;
                      wgint sz;
                      if (stat (filename, &st) == 0)
                        sz = st.st_size;
                      else
                        sz = -1;
                      logprintf (LOG_NOTQUIET,
                                 _("Wrote HTML-ized index to %s [%s].\n"),
                                 quote (filename), number_to_static_string (sz));
                    }
                  else
                    logprintf (LOG_NOTQUIET,
                               _("Wrote HTML-ized index to %s.\n"),
                               quote (filename));
                }
              xfree (filename);
            }
          freefileinfo (f);
        }
    }
  else
    {
      bool ispattern = false;
      if (glob)
        {
           
          char *file_part = strrchr (u->path, '/');
          if (!file_part)
            file_part = u->path;
          ispattern = has_wildcards_p (file_part);
        }
      if (ispattern || recursive || opt.timestamping || opt.preserve_perm)
        {
           
          res = ftp_retrieve_glob (u, &con,
                                   ispattern ? GLOB_GLOBALL : GLOB_GETONE);
        }
      else
        res = ftp_loop_internal (u, NULL, &con, local_file, false);
    }
  if (res == FTPOK)
    res = RETROK;
  if (res == RETROK)
    *dt |= RETROKF;
   
  if (con.csock != -1)
    fd_close (con.csock);
  xfree (con.id);
  xfree (con.target);
  return res;
}
