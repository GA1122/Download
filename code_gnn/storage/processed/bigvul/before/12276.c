gpgsm_keylist_ext (void *engine, const char *pattern[], int secret_only,
		   int reserved, gpgme_keylist_mode_t mode)
{
  engine_gpgsm_t gpgsm = engine;
  char *line;
  gpgme_error_t err;
   
  int length = 15 + 1;
  char *linep;
  int any_pattern = 0;
  int list_mode = 0;

  if (reserved)
    return gpg_error (GPG_ERR_INV_VALUE);

  if (mode & GPGME_KEYLIST_MODE_LOCAL)
    list_mode |= 1;
  if (mode & GPGME_KEYLIST_MODE_EXTERN)
    list_mode |= 2;

   
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
                               (mode & GPGME_KEYLIST_MODE_WITH_SECRET)?
                               "OPTION with-secret=1":
                               "OPTION with-secret=0" ,
                               NULL, NULL);


  if (pattern && *pattern)
    {
      const char **pat = pattern;

      while (*pat)
	{
	  const char *patlet = *pat;

	  while (*patlet)
	    {
	      length++;
	      if (*patlet == '%' || *patlet == ' ' || *patlet == '+')
		length += 2;
	      patlet++;
	    }
	  pat++;
	  length++;
	}
    }
  line = malloc (length);
  if (!line)
    return gpg_error_from_syserror ();
  if (secret_only)
    {
      strcpy (line, "LISTSECRETKEYS ");
      linep = &line[15];
    }
  else
    {
      strcpy (line, "LISTKEYS ");
      linep = &line[9];
    }

  if (pattern && *pattern)
    {
      while (*pattern)
	{
	  const char *patlet = *pattern;

	  while (*patlet)
	    {
	      switch (*patlet)
		{
		case '%':
		  *(linep++) = '%';
		  *(linep++) = '2';
		  *(linep++) = '5';
		  break;
		case ' ':
		  *(linep++) = '%';
		  *(linep++) = '2';
		  *(linep++) = '0';
		  break;
		case '+':
		  *(linep++) = '%';
		  *(linep++) = '2';
		  *(linep++) = 'B';
		  break;
		default:
		  *(linep++) = *patlet;
		  break;
		}
	      patlet++;
	    }
          any_pattern = 1;
          *linep++ = ' ';
	  pattern++;
	}
    }
  if (any_pattern)
    linep--;
  *linep = '\0';

  gpgsm_clear_fd (gpgsm, INPUT_FD);
  gpgsm_clear_fd (gpgsm, OUTPUT_FD);
  gpgsm_clear_fd (gpgsm, MESSAGE_FD);
  gpgsm->inline_data = NULL;

  err = start (gpgsm, line);
  free (line);
  return err;
}
