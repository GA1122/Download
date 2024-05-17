open_output_stream (struct http_stat *hs, int count, FILE **fp)
{
 
#ifdef __VMS
# define FOPEN_OPT_ARGS "fop=sqo", "acc", acc_cb, &open_id
# define FOPEN_BIN_FLAG 3
#else  
# define FOPEN_BIN_FLAG true
#endif  

   
  if (!output_stream)
    {
      mkalldirs (hs->local_file);
      if (opt.backups)
        rotate_backups (hs->local_file);
      if (hs->restval)
        {
#ifdef __VMS
          int open_id;

          open_id = 21;
          *fp = fopen (hs->local_file, "ab", FOPEN_OPT_ARGS);
#else  
          *fp = fopen (hs->local_file, "ab");
#endif  
        }
      else if (ALLOW_CLOBBER || count > 0)
        {
          if (opt.unlink_requested && file_exists_p (hs->local_file, NULL))
            {
              if (unlink (hs->local_file) < 0)
                {
                  logprintf (LOG_NOTQUIET, "%s: %s\n", hs->local_file,
                             strerror (errno));
                  return UNLINKERR;
                }
            }

#ifdef __VMS
          int open_id;

          open_id = 22;
          *fp = fopen (hs->local_file, "wb", FOPEN_OPT_ARGS);
#else  
          if (hs->temporary)
            {
              *fp = fdopen (open (hs->local_file, O_BINARY | O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR), "wb");
            }
          else
            {
              *fp = fopen (hs->local_file, "wb");
            }

#endif  
        }
      else
        {
          *fp = fopen_excl (hs->local_file, FOPEN_BIN_FLAG);
          if (!*fp && errno == EEXIST)
            {
               
              logprintf (LOG_NOTQUIET,
                         _("%s has sprung into existence.\n"),
                         hs->local_file);
              return FOPEN_EXCL_ERR;
            }
        }
      if (!*fp)
        {
          logprintf (LOG_NOTQUIET, "%s: %s\n", hs->local_file, strerror (errno));
          return FOPENERR;
        }
    }
  else
    *fp = output_stream;

   
  logprintf (LOG_VERBOSE, _("Saving to: %s\n"),
             HYPHENP (hs->local_file) ? quote ("STDOUT") : quote (hs->local_file));

  return RETROK;
}
