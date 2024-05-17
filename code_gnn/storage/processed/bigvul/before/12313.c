uiserver_sign (void *engine, gpgme_data_t in, gpgme_data_t out,
	       gpgme_sig_mode_t mode, int use_armor, int use_textmode,
	       int include_certs, gpgme_ctx_t ctx  )
{
  engine_uiserver_t uiserver = engine;
  gpgme_error_t err = 0;
  const char *protocol;
  char *cmd;
  gpgme_key_t key;

  if (!uiserver || !in || !out)
    return gpg_error (GPG_ERR_INV_VALUE);
  if (uiserver->protocol == GPGME_PROTOCOL_DEFAULT)
    protocol = "";
  else if (uiserver->protocol == GPGME_PROTOCOL_OpenPGP)
    protocol = " --protocol=OpenPGP";
  else if (uiserver->protocol == GPGME_PROTOCOL_CMS)
    protocol = " --protocol=CMS";
  else
    return gpgme_error (GPG_ERR_UNSUPPORTED_PROTOCOL);

  if (asprintf (&cmd, "SIGN%s%s", protocol,
		(mode == GPGME_SIG_MODE_DETACH) ? " --detached" : "") < 0)
    return gpg_error_from_syserror ();

  key = gpgme_signers_enum (ctx, 0);
  if (key)
    {
      const char *s = NULL;

      if (key && key->uids)
        s = key->uids->email;

      if (s && strlen (s) < 80)
        {
          char buf[100];

          strcpy (stpcpy (buf, "SENDER --info "), s);
          err = uiserver_assuan_simple_command (uiserver->assuan_ctx, buf,
                                                uiserver->status.fnc,
                                                uiserver->status.fnc_value);
        }
      else
        err = gpg_error (GPG_ERR_INV_VALUE);
      gpgme_key_unref (key);
      if (err)
      {
	free (cmd);
	return err;
      }
  }

  uiserver->input_cb.data = in;
  err = uiserver_set_fd (uiserver, INPUT_FD,
			 map_data_enc (uiserver->input_cb.data));
  if (err)
    {
      free (cmd);
      return err;
    }
  uiserver->output_cb.data = out;
  err = uiserver_set_fd (uiserver, OUTPUT_FD, use_armor ? "--armor"
			 : map_data_enc (uiserver->output_cb.data));
  if (err)
    {
      free (cmd);
      return err;
    }
  uiserver->inline_data = NULL;

  err = start (uiserver, cmd);
  free (cmd);
  return err;
}
