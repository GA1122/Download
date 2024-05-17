void nntp_article_status(struct Context *ctx, struct Header *hdr, char *group, anum_t anum)
{
  struct NntpData *nntp_data = ctx->data;

  if (group)
    nntp_data = mutt_hash_find(nntp_data->nserv->groups_hash, group);

  if (!nntp_data)
    return;

  for (unsigned int i = 0; i < nntp_data->newsrc_len; i++)
  {
    if ((anum >= nntp_data->newsrc_ent[i].first) &&
        (anum <= nntp_data->newsrc_ent[i].last))
    {
       
      hdr->read = true;
      return;
    }
  }

   
  if (anum > nntp_data->last_cached)
    return;

   
  if (MarkOld)
    hdr->old = true;
}
