ftp_retrieve_dirs (struct url *u, struct fileinfo *f, ccon *con)
{
  char *container = NULL;
  int container_size = 0;

  for (; f; f = f->next)
    {
      int size;
      char *odir, *newdir;

      if (opt.quota && total_downloaded_bytes > opt.quota)
        break;
      if (f->type != FT_DIRECTORY)
        continue;

       
      size = strlen (u->dir) + 1 + strlen (f->name) + 1;
      if (size > container_size)
        container = (char *)alloca (size);
      newdir = container;

      odir = u->dir;
      if (*odir == '\0'
          || (*odir == '/' && *(odir + 1) == '\0'))
         
        sprintf (newdir, "%s%s", odir, f->name);
      else
         
        sprintf (newdir, "%s/%s", odir, f->name);

      DEBUGP (("Composing new CWD relative to the initial directory.\n"));
      DEBUGP (("  odir = '%s'\n  f->name = '%s'\n  newdir = '%s'\n\n",
               odir, f->name, newdir));
      if (!accdir (newdir))
        {
          logprintf (LOG_VERBOSE, _("\
Not descending to %s as it is excluded/not-included.\n"),
                     quote (newdir));
          continue;
        }

      con->st &= ~DONE_CWD;

      odir = xstrdup (u->dir);   
      url_set_dir (u, newdir);
      ftp_retrieve_glob (u, con, GLOB_GETALL);
      url_set_dir (u, odir);
      xfree (odir);

       
    }

  if (opt.quota && total_downloaded_bytes > opt.quota)
    return QUOTEXC;
  else
    return RETROK;
}
