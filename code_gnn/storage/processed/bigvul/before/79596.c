static int imap_mbox_open(struct Context *ctx)
{
  struct ImapData *idata = NULL;
  struct ImapStatus *status = NULL;
  char buf[PATH_MAX];
  char bufout[PATH_MAX];
  int count = 0;
  struct ImapMbox mx, pmx;
  int rc;

  if (imap_parse_path(ctx->path, &mx))
  {
    mutt_error(_("%s is an invalid IMAP path"), ctx->path);
    return -1;
  }

   
  idata = imap_conn_find(&(mx.account), MUTT_IMAP_CONN_NOSELECT);
  if (!idata)
    goto fail_noidata;
  if (idata->state < IMAP_AUTHENTICATED)
    goto fail;

   
  ctx->data = idata;

   
  imap_fix_path(idata, mx.mbox, buf, sizeof(buf));
  if (!*buf)
    mutt_str_strfcpy(buf, "INBOX", sizeof(buf));
  FREE(&(idata->mailbox));
  idata->mailbox = mutt_str_strdup(buf);
  imap_qualify_path(buf, sizeof(buf), &mx, idata->mailbox);

  FREE(&(ctx->path));
  FREE(&(ctx->realpath));
  ctx->path = mutt_str_strdup(buf);
  ctx->realpath = mutt_str_strdup(ctx->path);

  idata->ctx = ctx;

   
  idata->status = false;
  memset(idata->ctx->rights, 0, sizeof(idata->ctx->rights));
  idata->new_mail_count = 0;
  idata->max_msn = 0;

  mutt_message(_("Selecting %s..."), idata->mailbox);
  imap_munge_mbox_name(idata, buf, sizeof(buf), idata->mailbox);

   
  if (mutt_bit_isset(idata->capabilities, ACL))
  {
    snprintf(bufout, sizeof(bufout), "MYRIGHTS %s", buf);
    imap_exec(idata, bufout, IMAP_CMD_QUEUE);
  }
   
  else
  {
    mutt_bit_set(idata->ctx->rights, MUTT_ACL_LOOKUP);
    mutt_bit_set(idata->ctx->rights, MUTT_ACL_READ);
    mutt_bit_set(idata->ctx->rights, MUTT_ACL_SEEN);
    mutt_bit_set(idata->ctx->rights, MUTT_ACL_WRITE);
    mutt_bit_set(idata->ctx->rights, MUTT_ACL_INSERT);
    mutt_bit_set(idata->ctx->rights, MUTT_ACL_POST);
    mutt_bit_set(idata->ctx->rights, MUTT_ACL_CREATE);
    mutt_bit_set(idata->ctx->rights, MUTT_ACL_DELETE);
  }
   
  pmx.mbox = NULL;
  if (mx_is_imap(Postponed) && !imap_parse_path(Postponed, &pmx) &&
      mutt_account_match(&pmx.account, &mx.account))
  {
    imap_status(Postponed, 1);
  }
  FREE(&pmx.mbox);

  if (ImapCheckSubscribed)
    imap_exec(idata, "LSUB \"\" \"*\"", IMAP_CMD_QUEUE);
  snprintf(bufout, sizeof(bufout), "%s %s", ctx->readonly ? "EXAMINE" : "SELECT", buf);

  idata->state = IMAP_SELECTED;

  imap_cmd_start(idata, bufout);

  status = imap_mboxcache_get(idata, idata->mailbox, 1);

  do
  {
    char *pc = NULL;

    rc = imap_cmd_step(idata);
    if (rc != IMAP_CMD_CONTINUE)
      break;

    pc = idata->buf + 2;

     
    if (mutt_str_strncasecmp("FLAGS", pc, 5) == 0)
    {
       
      if (STAILQ_EMPTY(&idata->flags))
      {
        mutt_debug(3, "Getting mailbox FLAGS\n");
        pc = get_flags(&idata->flags, pc);
        if (!pc)
          goto fail;
      }
    }
     
    else if (mutt_str_strncasecmp("OK [PERMANENTFLAGS", pc, 18) == 0)
    {
      mutt_debug(3, "Getting mailbox PERMANENTFLAGS\n");
       
      mutt_list_free(&idata->flags);
       
      pc += 13;
      pc = get_flags(&(idata->flags), pc);
      if (!pc)
        goto fail;
    }
     
    else if (mutt_str_strncasecmp("OK [UIDVALIDITY", pc, 14) == 0)
    {
      mutt_debug(3, "Getting mailbox UIDVALIDITY\n");
      pc += 3;
      pc = imap_next_word(pc);
      if (mutt_str_atoui(pc, &idata->uid_validity) < 0)
        goto fail;
      status->uidvalidity = idata->uid_validity;
    }
    else if (mutt_str_strncasecmp("OK [UIDNEXT", pc, 11) == 0)
    {
      mutt_debug(3, "Getting mailbox UIDNEXT\n");
      pc += 3;
      pc = imap_next_word(pc);
      if (mutt_str_atoui(pc, &idata->uidnext) < 0)
        goto fail;
      status->uidnext = idata->uidnext;
    }
    else
    {
      pc = imap_next_word(pc);
      if (mutt_str_strncasecmp("EXISTS", pc, 6) == 0)
      {
        count = idata->new_mail_count;
        idata->new_mail_count = 0;
      }
    }
  } while (rc == IMAP_CMD_CONTINUE);

  if (rc == IMAP_CMD_NO)
  {
    char *s = imap_next_word(idata->buf);  
    s = imap_next_word(s);                 
    mutt_error("%s", s);
    goto fail;
  }

  if (rc != IMAP_CMD_OK)
    goto fail;

   
  if ((mutt_str_strncasecmp(imap_get_qualifier(idata->buf), "[READ-ONLY]", 11) == 0) &&
      !mutt_bit_isset(idata->capabilities, ACL))
  {
    mutt_debug(2, "Mailbox is read-only.\n");
    ctx->readonly = true;
  }

   
  if (DebugLevel > 2)
  {
    if (STAILQ_EMPTY(&idata->flags))
      mutt_debug(3, "No folder flags found\n");
    else
    {
      struct ListNode *np;
      struct Buffer flag_buffer;
      mutt_buffer_init(&flag_buffer);
      mutt_buffer_printf(&flag_buffer, "Mailbox flags: ");
      STAILQ_FOREACH(np, &idata->flags, entries)
      {
        mutt_buffer_printf(&flag_buffer, "[%s] ", np->data);
      }
      mutt_debug(3, "%s\n", flag_buffer.data);
      FREE(&flag_buffer.data);
    }
  }

  if (!(mutt_bit_isset(idata->ctx->rights, MUTT_ACL_DELETE) ||
        mutt_bit_isset(idata->ctx->rights, MUTT_ACL_SEEN) ||
        mutt_bit_isset(idata->ctx->rights, MUTT_ACL_WRITE) ||
        mutt_bit_isset(idata->ctx->rights, MUTT_ACL_INSERT)))
  {
    ctx->readonly = true;
  }

  ctx->hdrmax = count;
  ctx->hdrs = mutt_mem_calloc(count, sizeof(struct Header *));
  ctx->v2r = mutt_mem_calloc(count, sizeof(int));
  ctx->msgcount = 0;

  if (count && (imap_read_headers(idata, 1, count) < 0))
  {
    mutt_error(_("Error opening mailbox"));
    goto fail;
  }

  mutt_debug(2, "msgcount is %d\n", ctx->msgcount);
  FREE(&mx.mbox);
  return 0;

fail:
  if (idata->state == IMAP_SELECTED)
    idata->state = IMAP_AUTHENTICATED;
fail_noidata:
  FREE(&mx.mbox);
  return -1;
}