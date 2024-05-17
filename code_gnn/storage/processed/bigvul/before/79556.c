static void cmd_parse_expunge(struct ImapData *idata, const char *s)
{
  unsigned int exp_msn;
  struct Header *h = NULL;

  mutt_debug(2, "Handling EXPUNGE\n");

  if (mutt_str_atoui(s, &exp_msn) < 0 || exp_msn < 1 || exp_msn > idata->max_msn)
    return;

  h = idata->msn_index[exp_msn - 1];
  if (h)
  {
     
    h->index = INT_MAX;
    HEADER_DATA(h)->msn = 0;
  }

   
  for (unsigned int cur = exp_msn; cur < idata->max_msn; cur++)
  {
    h = idata->msn_index[cur];
    if (h)
      HEADER_DATA(h)->msn--;
    idata->msn_index[cur - 1] = h;
  }

  idata->msn_index[idata->max_msn - 1] = NULL;
  idata->max_msn--;

  idata->reopen |= IMAP_EXPUNGE_PENDING;
}
