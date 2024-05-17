int imap_check(struct ImapData *idata, int force)
{
   
  int result = 0;

   
  if (!force && ImapIdle && mutt_bit_isset(idata->capabilities, IDLE) &&
      (idata->state != IMAP_IDLE || time(NULL) >= idata->lastread + ImapKeepalive))
  {
    if (imap_cmd_idle(idata) < 0)
      return -1;
  }
  if (idata->state == IMAP_IDLE)
  {
    while ((result = mutt_socket_poll(idata->conn, 0)) > 0)
    {
      if (imap_cmd_step(idata) != IMAP_CMD_CONTINUE)
      {
        mutt_debug(1, "Error reading IDLE response\n");
        return -1;
      }
    }
    if (result < 0)
    {
      mutt_debug(1, "Poll failed, disabling IDLE\n");
      mutt_bit_unset(idata->capabilities, IDLE);
    }
  }

  if ((force || (idata->state != IMAP_IDLE && time(NULL) >= idata->lastread + Timeout)) &&
      imap_exec(idata, "NOOP", IMAP_CMD_POLL) != 0)
  {
    return -1;
  }

   
  imap_cmd_finish(idata);

  if (idata->check_status & IMAP_EXPUNGE_PENDING)
    result = MUTT_REOPENED;
  else if (idata->check_status & IMAP_NEWMAIL_PENDING)
    result = MUTT_NEW_MAIL;
  else if (idata->check_status & IMAP_FLAGS_PENDING)
    result = MUTT_FLAGS;

  idata->check_status = 0;

  return result;
}
