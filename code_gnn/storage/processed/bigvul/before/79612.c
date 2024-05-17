static int make_msg_set(struct ImapData *idata, struct Buffer *buf, int flag,
                        bool changed, bool invert, int *pos)
{
  int count = 0;              
  unsigned int setstart = 0;  
  int n;
  bool started = false;
  struct Header **hdrs = idata->ctx->hdrs;

  for (n = *pos; n < idata->ctx->msgcount && buf->dptr - buf->data < IMAP_MAX_CMDLEN; n++)
  {
    bool match = false;  
     
    if (hdrs[n]->active)
    {
      switch (flag)
      {
        case MUTT_DELETED:
          if (hdrs[n]->deleted != HEADER_DATA(hdrs[n])->deleted)
            match = invert ^ hdrs[n]->deleted;
          break;
        case MUTT_FLAG:
          if (hdrs[n]->flagged != HEADER_DATA(hdrs[n])->flagged)
            match = invert ^ hdrs[n]->flagged;
          break;
        case MUTT_OLD:
          if (hdrs[n]->old != HEADER_DATA(hdrs[n])->old)
            match = invert ^ hdrs[n]->old;
          break;
        case MUTT_READ:
          if (hdrs[n]->read != HEADER_DATA(hdrs[n])->read)
            match = invert ^ hdrs[n]->read;
          break;
        case MUTT_REPLIED:
          if (hdrs[n]->replied != HEADER_DATA(hdrs[n])->replied)
            match = invert ^ hdrs[n]->replied;
          break;
        case MUTT_TAG:
          if (hdrs[n]->tagged)
            match = true;
          break;
        case MUTT_TRASH:
          if (hdrs[n]->deleted && !hdrs[n]->purge)
            match = true;
          break;
      }
    }

    if (match && (!changed || hdrs[n]->changed))
    {
      count++;
      if (setstart == 0)
      {
        setstart = HEADER_DATA(hdrs[n])->uid;
        if (!started)
        {
          mutt_buffer_printf(buf, "%u", HEADER_DATA(hdrs[n])->uid);
          started = true;
        }
        else
          mutt_buffer_printf(buf, ",%u", HEADER_DATA(hdrs[n])->uid);
      }
       
      else if (n == idata->ctx->msgcount - 1)
        mutt_buffer_printf(buf, ":%u", HEADER_DATA(hdrs[n])->uid);
    }
     
    else if (setstart && (hdrs[n]->active || n == idata->ctx->msgcount - 1))
    {
      if (HEADER_DATA(hdrs[n - 1])->uid > setstart)
        mutt_buffer_printf(buf, ":%u", HEADER_DATA(hdrs[n - 1])->uid);
      setstart = 0;
    }
  }

  *pos = n;

  return count;
}
