gpgsm_keylist (void *engine, const char *pattern, int secret_only,
	       gpgme_keylist_mode_t mode)
{
  engine_gpgsm_t gpgsm = engine;
  char *line;
  gpgme_error_t err;
  int list_mode = 0;

  if (mode & GPGME_KEYLIST_MODE_LOCAL)
    list_mode |= 1;
  if (mode & GPGME_KEYLIST_MODE_EXTERN)
    list_mode |= 2;

  if (!pattern)
    pattern = "";

   
  if (secret_only || (mode & GPGME_KEYLIST_MODE_WITH_SECRET))
    gpgsm_assuan_simple_command (gpgsm->assuan_ctx, "GETINFO agent-check",
                                 NULL, NULL);

   
  if (asprintf (&line, "OPTION list-mode=%d", (list_mode & 3)) < 0)
    return gpg_error_from_syserror ();
  err = gpgsm_assuan_simple_command (gpgsm->assuan_ctx, line, NULL, NULL);
  free (line);
  if (err)
    return err;


   

   
  gpgsm_assuan_simple_command (gpgsm->assuan_ctx,
                               (mode & GPGME_KEYLIST_MODE_VALIDATE)?
                               "OPTION with-validation=1":
                               "OPTION with-validation=0" ,
                               NULL, NULL);
   
  gpgsm_assuan_simple_command (gpgsm->assuan_ctx,
                               (mode & GPGME_KEYLIST_MODE_EPHEMERAL)?
                               "OPTION with-ephemeral-keys=1":
                               "OPTION with-ephemeral-keys=0" ,
                               NULL, NULL);
  gpgsm_assuan_simple_command (gpgsm->assuan_ctx,
                               (mode & GPGME_KEYLIST_MODE_WITH_SECRET)?
                               "OPTION with-secret=1":
                               "OPTION with-secret=0" ,
                               NULL, NULL);


   
  line = malloc (15 + strlen (pattern) + 1);
  if (!line)
    return gpg_error_from_syserror ();
  if (secret_only)
    {
      strcpy (line, "LISTSECRETKEYS ");
      strcpy (&line[15], pattern);
    }
  else
    {
      strcpy (line, "LISTKEYS ");
      strcpy (&line[9], pattern);
    }

  gpgsm_clear_fd (gpgsm, INPUT_FD);
  gpgsm_clear_fd (gpgsm, OUTPUT_FD);
  gpgsm_clear_fd (gpgsm, MESSAGE_FD);
  gpgsm->inline_data = NULL;

  err = start (gpgsm, line);
  free (line);
  return err;
}
