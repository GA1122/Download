int imap_cmd_idle(struct ImapData *idata)
{
  int rc;

  if (cmd_start(idata, "IDLE", IMAP_CMD_POLL) < 0)
  {
    cmd_handle_fatal(idata);
    return -1;
  }

  if ((ImapPollTimeout > 0) && (mutt_socket_poll(idata->conn, ImapPollTimeout)) == 0)
  {
    mutt_error(_("Connection to %s timed out"), idata->conn->account.host);
    cmd_handle_fatal(idata);
    return -1;
  }

  do
    rc = imap_cmd_step(idata);
  while (rc == IMAP_CMD_CONTINUE);

  if (rc == IMAP_CMD_RESPOND)
  {
     
    idata->state = IMAP_IDLE;
     
    mutt_buffer_printf(idata->cmdbuf, "DONE\r\n");
    rc = IMAP_CMD_OK;
  }
  if (rc != IMAP_CMD_OK)
  {
    mutt_debug(1, "error starting IDLE\n");
    return -1;
  }

  return 0;
}