int imap_exec_msgset(struct ImapData *idata, const char *pre, const char *post,
                     int flag, int changed, int invert)
{
  struct Header **hdrs = NULL;
  short oldsort;
  int pos;
  int rc;
  int count = 0;

  struct Buffer *cmd = mutt_buffer_new();

   
  oldsort = Sort;
  if (Sort != SORT_ORDER)
  {
    hdrs = idata->ctx->hdrs;
    idata->ctx->hdrs = mutt_mem_malloc(idata->ctx->msgcount * sizeof(struct Header *));
    memcpy(idata->ctx->hdrs, hdrs, idata->ctx->msgcount * sizeof(struct Header *));

    Sort = SORT_ORDER;
    qsort(idata->ctx->hdrs, idata->ctx->msgcount, sizeof(struct Header *),
          mutt_get_sort_func(SORT_ORDER));
  }

  pos = 0;

  do
  {
    cmd->dptr = cmd->data;
    mutt_buffer_printf(cmd, "%s ", pre);
    rc = make_msg_set(idata, cmd, flag, changed, invert, &pos);
    if (rc > 0)
    {
      mutt_buffer_printf(cmd, " %s", post);
      if (imap_exec(idata, cmd->data, IMAP_CMD_QUEUE))
      {
        rc = -1;
        goto out;
      }
      count += rc;
    }
  } while (rc > 0);

  rc = count;

out:
  mutt_buffer_free(&cmd);
  if (oldsort != Sort)
  {
    Sort = oldsort;
    FREE(&idata->ctx->hdrs);
    idata->ctx->hdrs = hdrs;
  }

  return rc;
}