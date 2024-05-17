struct ImapStatus *imap_mboxcache_get(struct ImapData *idata, const char *mbox, bool create)
{
  struct ImapStatus *status = NULL;
#ifdef USE_HCACHE
  header_cache_t *hc = NULL;
  void *uidvalidity = NULL;
  void *uidnext = NULL;
#endif

  struct ListNode *np;
  STAILQ_FOREACH(np, &idata->mboxcache, entries)
  {
    status = (struct ImapStatus *) np->data;

    if (imap_mxcmp(mbox, status->name) == 0)
      return status;
  }
  status = NULL;

   
  if (create)
  {
    struct ImapStatus *scache = mutt_mem_calloc(1, sizeof(struct ImapStatus));
    scache->name = (char *) mbox;
    mutt_list_insert_tail(&idata->mboxcache, (char *) scache);
    status = imap_mboxcache_get(idata, mbox, 0);
    status->name = mutt_str_strdup(mbox);
  }

#ifdef USE_HCACHE
  hc = imap_hcache_open(idata, mbox);
  if (hc)
  {
    uidvalidity = mutt_hcache_fetch_raw(hc, "/UIDVALIDITY", 12);
    uidnext = mutt_hcache_fetch_raw(hc, "/UIDNEXT", 8);
    if (uidvalidity)
    {
      if (!status)
      {
        mutt_hcache_free(hc, &uidvalidity);
        mutt_hcache_free(hc, &uidnext);
        mutt_hcache_close(hc);
        return imap_mboxcache_get(idata, mbox, 1);
      }
      status->uidvalidity = *(unsigned int *) uidvalidity;
      status->uidnext = uidnext ? *(unsigned int *) uidnext : 0;
      mutt_debug(3, "hcache uidvalidity %u, uidnext %u\n", status->uidvalidity,
                 status->uidnext);
    }
    mutt_hcache_free(hc, &uidvalidity);
    mutt_hcache_free(hc, &uidnext);
    mutt_hcache_close(hc);
  }
#endif

  return status;
}