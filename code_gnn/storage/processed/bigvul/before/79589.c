void imap_expunge_mailbox(struct ImapData *idata)
{
  struct Header *h = NULL;
  int cacheno;
  short old_sort;

#ifdef USE_HCACHE
  idata->hcache = imap_hcache_open(idata, NULL);
#endif

  old_sort = Sort;
  Sort = SORT_ORDER;
  mutt_sort_headers(idata->ctx, 0);

  for (int i = 0; i < idata->ctx->msgcount; i++)
  {
    h = idata->ctx->hdrs[i];

    if (h->index == INT_MAX)
    {
      mutt_debug(2, "Expunging message UID %u.\n", HEADER_DATA(h)->uid);

      h->active = false;
      idata->ctx->size -= h->content->length;

      imap_cache_del(idata, h);
#ifdef USE_HCACHE
      imap_hcache_del(idata, HEADER_DATA(h)->uid);
#endif

       
      cacheno = HEADER_DATA(h)->uid % IMAP_CACHE_LEN;
      if (idata->cache[cacheno].uid == HEADER_DATA(h)->uid &&
          idata->cache[cacheno].path)
      {
        unlink(idata->cache[cacheno].path);
        FREE(&idata->cache[cacheno].path);
      }

      mutt_hash_int_delete(idata->uid_hash, HEADER_DATA(h)->uid, h);

      imap_free_header_data((struct ImapHeaderData **) &h->data);
    }
    else
    {
      h->index = i;
       
      h->active = true;
    }
  }

#ifdef USE_HCACHE
  imap_hcache_close(idata);
#endif

   
  mx_update_tables(idata->ctx, false);
  Sort = old_sort;
  mutt_sort_headers(idata->ctx, 1);
}
