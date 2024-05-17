int imap_fast_trash(struct Context *ctx, char *dest)
{
  char mbox[LONG_STRING];
  char mmbox[LONG_STRING];
  char prompt[LONG_STRING];
  int rc;
  struct ImapMbox mx;
  bool triedcreate = false;
  struct Buffer *sync_cmd = NULL;
  int err_continue = MUTT_NO;

  struct ImapData *idata = ctx->data;

  if (imap_parse_path(dest, &mx))
  {
    mutt_debug(1, "bad destination %s\n", dest);
    return -1;
  }

   
  if (mutt_account_match(&(idata->conn->account), &(mx.account)) == 0)
  {
    mutt_debug(3, "%s not same server as %s\n", dest, ctx->path);
    return 1;
  }

  imap_fix_path(idata, mx.mbox, mbox, sizeof(mbox));
  if (!*mbox)
    mutt_str_strfcpy(mbox, "INBOX", sizeof(mbox));
  imap_munge_mbox_name(idata, mmbox, sizeof(mmbox), mbox);

  sync_cmd = mutt_buffer_new();
  for (int i = 0; i < ctx->msgcount; i++)
  {
    if (ctx->hdrs[i]->active && ctx->hdrs[i]->changed &&
        ctx->hdrs[i]->deleted && !ctx->hdrs[i]->purge)
    {
      rc = imap_sync_message_for_copy(idata, ctx->hdrs[i], sync_cmd, &err_continue);
      if (rc < 0)
      {
        mutt_debug(1, "could not sync\n");
        goto out;
      }
    }
  }

   
  do
  {
    rc = imap_exec_msgset(idata, "UID COPY", mmbox, MUTT_TRASH, 0, 0);
    if (!rc)
    {
      mutt_debug(1, "No messages to trash\n");
      rc = -1;
      goto out;
    }
    else if (rc < 0)
    {
      mutt_debug(1, "could not queue copy\n");
      goto out;
    }
    else
    {
      mutt_message(ngettext("Copying %d message to %s...", "Copying %d messages to %s...", rc),
                   rc, mbox);
    }

     
    rc = imap_exec(idata, NULL, IMAP_CMD_FAIL_OK);
    if (rc == -2)
    {
      if (triedcreate)
      {
        mutt_debug(1, "Already tried to create mailbox %s\n", mbox);
        break;
      }
       
      if (mutt_str_strncasecmp(imap_get_qualifier(idata->buf), "[TRYCREATE]", 11) != 0)
        break;
      mutt_debug(3, "server suggests TRYCREATE\n");
      snprintf(prompt, sizeof(prompt), _("Create %s?"), mbox);
      if (Confirmcreate && mutt_yesorno(prompt, 1) != MUTT_YES)
      {
        mutt_clear_error();
        goto out;
      }
      if (imap_create_mailbox(idata, mbox) < 0)
        break;
      triedcreate = true;
    }
  } while (rc == -2);

  if (rc != 0)
  {
    imap_error("imap_fast_trash", idata->buf);
    goto out;
  }

  rc = 0;

out:
  mutt_buffer_free(&sync_cmd);
  FREE(&mx.mbox);

  return (rc < 0) ? -1 : rc;
}