static int imap_mbox_close(struct Context *ctx)
{
  struct ImapData *idata = ctx->data;
   
  if (!idata)
    return 0;

   
  if (ctx == idata->ctx)
  {
    if (idata->status != IMAP_FATAL && idata->state >= IMAP_SELECTED)
    {
       
      if (!ctx->deleted)
        imap_exec(idata, "CLOSE", IMAP_CMD_QUEUE);
      idata->state = IMAP_AUTHENTICATED;
    }

    idata->reopen &= IMAP_REOPEN_ALLOW;
    FREE(&(idata->mailbox));
    mutt_list_free(&idata->flags);
    idata->ctx = NULL;

    mutt_hash_destroy(&idata->uid_hash);
    FREE(&idata->msn_index);
    idata->msn_index_size = 0;
    idata->max_msn = 0;

    for (int i = 0; i < IMAP_CACHE_LEN; i++)
    {
      if (idata->cache[i].path)
      {
        unlink(idata->cache[i].path);
        FREE(&idata->cache[i].path);
      }
    }

    mutt_bcache_close(&idata->bcache);
  }

   
  for (int i = 0; i < ctx->msgcount; i++)
  {
     
    if (ctx->hdrs[i] && ctx->hdrs[i]->data)
      imap_free_header_data((struct ImapHeaderData **) &(ctx->hdrs[i]->data));
  }

  return 0;
}
