static int imap_mbox_open_append(struct Context *ctx, int flags)
{
  struct ImapData *idata = NULL;
  char mailbox[PATH_MAX];
  struct ImapMbox mx;
  int rc;

  if (imap_parse_path(ctx->path, &mx))
    return -1;

   

  idata = imap_conn_find(&(mx.account), 0);
  if (!idata)
  {
    FREE(&mx.mbox);
    return -1;
  }

  ctx->data = idata;

  imap_fix_path(idata, mx.mbox, mailbox, sizeof(mailbox));
  if (!*mailbox)
    mutt_str_strfcpy(mailbox, "INBOX", sizeof(mailbox));
  FREE(&mx.mbox);

  rc = imap_access(ctx->path);
  if (rc == 0)
    return 0;

  if (rc == -1)
    return -1;

  char buf[PATH_MAX + 64];
  snprintf(buf, sizeof(buf), _("Create %s?"), mailbox);
  if (Confirmcreate && mutt_yesorno(buf, 1) != MUTT_YES)
    return -1;

  if (imap_create_mailbox(idata, mailbox) < 0)
    return -1;

  return 0;
}