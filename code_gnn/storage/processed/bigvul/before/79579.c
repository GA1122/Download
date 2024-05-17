int imap_access(const char *path)
{
  struct ImapData *idata = NULL;
  struct ImapMbox mx;
  char buf[LONG_STRING];
  char mailbox[LONG_STRING];
  char mbox[LONG_STRING];
  int rc;

  if (imap_parse_path(path, &mx))
    return -1;

  idata = imap_conn_find(&mx.account, ImapPassive ? MUTT_IMAP_CONN_NONEW : 0);
  if (!idata)
  {
    FREE(&mx.mbox);
    return -1;
  }

  imap_fix_path(idata, mx.mbox, mailbox, sizeof(mailbox));
  if (!*mailbox)
    mutt_str_strfcpy(mailbox, "INBOX", sizeof(mailbox));

   
  if (mutt_str_strcmp(idata->mailbox, mx.mbox) == 0)
  {
    FREE(&mx.mbox);
    return 0;
  }
  FREE(&mx.mbox);

  if (imap_mboxcache_get(idata, mailbox, 0))
  {
    mutt_debug(3, "found %s in cache\n", mailbox);
    return 0;
  }

  imap_munge_mbox_name(idata, mbox, sizeof(mbox), mailbox);

  if (mutt_bit_isset(idata->capabilities, IMAP4REV1))
    snprintf(buf, sizeof(buf), "STATUS %s (UIDVALIDITY)", mbox);
  else if (mutt_bit_isset(idata->capabilities, STATUS))
    snprintf(buf, sizeof(buf), "STATUS %s (UID-VALIDITY)", mbox);
  else
  {
    mutt_debug(2, "STATUS not supported?\n");
    return -1;
  }

  rc = imap_exec(idata, buf, IMAP_CMD_FAIL_OK);
  if (rc < 0)
  {
    mutt_debug(1, "Can't check STATUS of %s\n", mbox);
    return rc;
  }

  return 0;
}