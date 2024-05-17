void imap_cmd_finish(struct ImapData *idata)
{
  if (idata->status == IMAP_FATAL)
  {
    cmd_handle_fatal(idata);
    return;
  }

  if (!(idata->state >= IMAP_SELECTED) || idata->ctx->closing)
    return;

  if (idata->reopen & IMAP_REOPEN_ALLOW)
  {
    unsigned int count = idata->new_mail_count;

    if (!(idata->reopen & IMAP_EXPUNGE_PENDING) &&
        (idata->reopen & IMAP_NEWMAIL_PENDING) && count > idata->max_msn)
    {
       
      mutt_debug(2, "Fetching new mail\n");
       
      idata->check_status = IMAP_NEWMAIL_PENDING;
      imap_read_headers(idata, idata->max_msn + 1, count);
    }
    else if (idata->reopen & IMAP_EXPUNGE_PENDING)
    {
      mutt_debug(2, "Expunging mailbox\n");
      imap_expunge_mailbox(idata);
       
      if ((idata->reopen & IMAP_EXPUNGE_PENDING) && !(idata->reopen & IMAP_EXPUNGE_EXPECTED))
        idata->check_status = IMAP_EXPUNGE_PENDING;
      idata->reopen &=
          ~(IMAP_EXPUNGE_PENDING | IMAP_NEWMAIL_PENDING | IMAP_EXPUNGE_EXPECTED);
    }
  }

  idata->status = false;
}
