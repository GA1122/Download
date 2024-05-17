int imap_status(char *path, int queue)
{
  static int queued = 0;

  struct ImapData *idata = NULL;
  char buf[LONG_STRING];
  char mbox[LONG_STRING];
  struct ImapStatus *status = NULL;

  if (get_mailbox(path, &idata, buf, sizeof(buf)) < 0)
    return -1;

   
  if (idata->ctx && !imap_mxcmp(buf, idata->mailbox))
    return idata->ctx->msgcount;
  else if (mutt_bit_isset(idata->capabilities, IMAP4REV1) ||
           mutt_bit_isset(idata->capabilities, STATUS))
  {
    imap_munge_mbox_name(idata, mbox, sizeof(mbox), buf);
    snprintf(buf, sizeof(buf), "STATUS %s (%s)", mbox, "MESSAGES");
    imap_unmunge_mbox_name(idata, mbox);
  }
  else
  {
     
    return -1;
  }

  if (queue)
  {
    imap_exec(idata, buf, IMAP_CMD_QUEUE);
    queued = 1;
    return 0;
  }
  else if (!queued)
    imap_exec(idata, buf, 0);

  queued = 0;
  status = imap_mboxcache_get(idata, mbox, 0);
  if (status)
    return status->messages;

  return 0;
}
