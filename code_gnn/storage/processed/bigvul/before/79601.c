int imap_open_connection(struct ImapData *idata)
{
  char buf[LONG_STRING];

  if (mutt_socket_open(idata->conn) < 0)
    return -1;

  idata->state = IMAP_CONNECTED;

  if (imap_cmd_step(idata) != IMAP_CMD_OK)
  {
    imap_close_connection(idata);
    return -1;
  }

  if (mutt_str_strncasecmp("* OK", idata->buf, 4) == 0)
  {
    if ((mutt_str_strncasecmp("* OK [CAPABILITY", idata->buf, 16) != 0) &&
        check_capabilities(idata))
    {
      goto bail;
    }
#ifdef USE_SSL
     
    if (!idata->conn->ssf && (SslForceTls || mutt_bit_isset(idata->capabilities, STARTTLS)))
    {
      int rc;

      if (SslForceTls)
        rc = MUTT_YES;
      else if ((rc = query_quadoption(SslStarttls,
                                      _("Secure connection with TLS?"))) == MUTT_ABORT)
      {
        goto err_close_conn;
      }
      if (rc == MUTT_YES)
      {
        rc = imap_exec(idata, "STARTTLS", IMAP_CMD_FAIL_OK);
        if (rc == -1)
          goto bail;
        if (rc != -2)
        {
          if (mutt_ssl_starttls(idata->conn))
          {
            mutt_error(_("Could not negotiate TLS connection"));
            goto err_close_conn;
          }
          else
          {
             
            if (imap_exec(idata, "CAPABILITY", 0))
              goto bail;
          }
        }
      }
    }

    if (SslForceTls && !idata->conn->ssf)
    {
      mutt_error(_("Encrypted connection unavailable"));
      goto err_close_conn;
    }
#endif
  }
  else if (mutt_str_strncasecmp("* PREAUTH", idata->buf, 9) == 0)
  {
    idata->state = IMAP_AUTHENTICATED;
    if (check_capabilities(idata) != 0)
      goto bail;
    FREE(&idata->capstr);
  }
  else
  {
    imap_error("imap_open_connection()", buf);
    goto bail;
  }

  return 0;

#ifdef USE_SSL
err_close_conn:
  imap_close_connection(idata);
#endif
bail:
  FREE(&idata->capstr);
  return -1;
}