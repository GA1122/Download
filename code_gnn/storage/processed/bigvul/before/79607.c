int imap_sync_mailbox(struct Context *ctx, int expunge)
{
  struct Context *appendctx = NULL;
  struct Header *h = NULL;
  struct Header **hdrs = NULL;
  int oldsort;
  int rc;

  struct ImapData *idata = ctx->data;

  if (idata->state < IMAP_SELECTED)
  {
    mutt_debug(2, "no mailbox selected\n");
    return -1;
  }

   
  imap_allow_reopen(ctx);

  rc = imap_check(idata, 0);
  if (rc != 0)
    return rc;

   
  if (expunge && mutt_bit_isset(ctx->rights, MUTT_ACL_DELETE))
  {
    rc = imap_exec_msgset(idata, "UID STORE", "+FLAGS.SILENT (\\Deleted)",
                          MUTT_DELETED, 1, 0);
    if (rc < 0)
    {
      mutt_error(_("Expunge failed"));
      goto out;
    }

    if (rc > 0)
    {
       
      for (int i = 0; i < ctx->msgcount; i++)
        if (ctx->hdrs[i]->deleted && ctx->hdrs[i]->changed)
          ctx->hdrs[i]->active = false;
      mutt_message(ngettext("Marking %d message deleted...",
                            "Marking %d messages deleted...", rc),
                   rc);
    }
  }

#ifdef USE_HCACHE
  idata->hcache = imap_hcache_open(idata, NULL);
#endif

   
  for (int i = 0; i < ctx->msgcount; i++)
  {
    h = ctx->hdrs[i];

    if (h->deleted)
    {
      imap_cache_del(idata, h);
#ifdef USE_HCACHE
      imap_hcache_del(idata, HEADER_DATA(h)->uid);
#endif
    }

    if (h->active && h->changed)
    {
#ifdef USE_HCACHE
      imap_hcache_put(idata, h);
#endif
       
      if ((h->env && (h->env->refs_changed || h->env->irt_changed)) ||
          h->attach_del || h->xlabel_changed)
      {
         
        mutt_message(ngettext("Saving changed message... [%d/%d]",
                              "Saving changed messages... [%d/%d]", ctx->msgcount),
                     i + 1, ctx->msgcount);
        if (!appendctx)
          appendctx = mx_mbox_open(ctx->path, MUTT_APPEND | MUTT_QUIET, NULL);
        if (!appendctx)
          mutt_debug(1, "Error opening mailbox in append mode\n");
        else
          mutt_save_message_ctx(h, 1, 0, 0, appendctx);
        h->xlabel_changed = false;
      }
    }
  }

#ifdef USE_HCACHE
  imap_hcache_close(idata);
#endif

   
  oldsort = Sort;
  if (Sort != SORT_ORDER)
  {
    hdrs = ctx->hdrs;
    ctx->hdrs = mutt_mem_malloc(ctx->msgcount * sizeof(struct Header *));
    memcpy(ctx->hdrs, hdrs, ctx->msgcount * sizeof(struct Header *));

    Sort = SORT_ORDER;
    qsort(ctx->hdrs, ctx->msgcount, sizeof(struct Header *), mutt_get_sort_func(SORT_ORDER));
  }

  rc = sync_helper(idata, MUTT_ACL_DELETE, MUTT_DELETED, "\\Deleted");
  if (rc >= 0)
    rc |= sync_helper(idata, MUTT_ACL_WRITE, MUTT_FLAG, "\\Flagged");
  if (rc >= 0)
    rc |= sync_helper(idata, MUTT_ACL_WRITE, MUTT_OLD, "Old");
  if (rc >= 0)
    rc |= sync_helper(idata, MUTT_ACL_SEEN, MUTT_READ, "\\Seen");
  if (rc >= 0)
    rc |= sync_helper(idata, MUTT_ACL_WRITE, MUTT_REPLIED, "\\Answered");

  if (oldsort != Sort)
  {
    Sort = oldsort;
    FREE(&ctx->hdrs);
    ctx->hdrs = hdrs;
  }

   
  if (rc > 0)
    if (imap_exec(idata, NULL, 0) != IMAP_CMD_OK)
      rc = -1;

  if (rc < 0)
  {
    if (ctx->closing)
    {
      if (mutt_yesorno(_("Error saving flags. Close anyway?"), 0) == MUTT_YES)
      {
        rc = 0;
        idata->state = IMAP_AUTHENTICATED;
        goto out;
      }
    }
    else
      mutt_error(_("Error saving flags"));
    rc = -1;
    goto out;
  }

   
  for (int i = 0; i < ctx->msgcount; i++)
  {
    HEADER_DATA(ctx->hdrs[i])->deleted = ctx->hdrs[i]->deleted;
    HEADER_DATA(ctx->hdrs[i])->flagged = ctx->hdrs[i]->flagged;
    HEADER_DATA(ctx->hdrs[i])->old = ctx->hdrs[i]->old;
    HEADER_DATA(ctx->hdrs[i])->read = ctx->hdrs[i]->read;
    HEADER_DATA(ctx->hdrs[i])->replied = ctx->hdrs[i]->replied;
    ctx->hdrs[i]->changed = false;
  }
  ctx->changed = false;

   
  if (expunge && !(ctx->closing) && mutt_bit_isset(ctx->rights, MUTT_ACL_DELETE))
  {
    mutt_message(_("Expunging messages from server..."));
     
    idata->reopen |= IMAP_EXPUNGE_EXPECTED;
    if (imap_exec(idata, "EXPUNGE", 0) != 0)
    {
      idata->reopen &= ~IMAP_EXPUNGE_EXPECTED;
      imap_error(_("imap_sync_mailbox: EXPUNGE failed"), idata->buf);
      rc = -1;
      goto out;
    }
    idata->reopen &= ~IMAP_EXPUNGE_EXPECTED;
  }

  if (expunge && ctx->closing)
  {
    imap_exec(idata, "CLOSE", IMAP_CMD_QUEUE);
    idata->state = IMAP_AUTHENTICATED;
  }

  if (MessageCacheClean)
    imap_cache_clean(idata);

  rc = 0;

out:
  if (appendctx)
  {
    mx_fastclose_mailbox(appendctx);
    FREE(&appendctx);
  }
  return rc;
}
