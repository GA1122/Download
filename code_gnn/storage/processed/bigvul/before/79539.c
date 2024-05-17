int imap_read_headers(struct ImapData *idata, unsigned int msn_begin, unsigned int msn_end)
{
  char *hdrreq = NULL;
  FILE *fp = NULL;
  char tempfile[PATH_MAX];
  int msgno, idx;
  struct ImapHeader h;
  struct ImapStatus *status = NULL;
  int rc, mfhrc = 0, oldmsgcount;
  int fetch_msn_end = 0;
  unsigned int maxuid = 0;
  static const char *const want_headers =
      "DATE FROM SUBJECT TO CC MESSAGE-ID REFERENCES CONTENT-TYPE "
      "CONTENT-DESCRIPTION IN-REPLY-TO REPLY-TO LINES LIST-POST X-LABEL "
      "X-ORIGINAL-TO";
  struct Progress progress;
  int retval = -1;
  bool evalhc = false;

#ifdef USE_HCACHE
  char buf[LONG_STRING];
  void *uid_validity = NULL;
  void *puidnext = NULL;
  unsigned int uidnext = 0;
#endif  

  struct Context *ctx = idata->ctx;

  if (mutt_bit_isset(idata->capabilities, IMAP4REV1))
  {
    safe_asprintf(&hdrreq, "BODY.PEEK[HEADER.FIELDS (%s%s%s)]", want_headers,
                  ImapHeaders ? " " : "", NONULL(ImapHeaders));
  }
  else if (mutt_bit_isset(idata->capabilities, IMAP4))
  {
    safe_asprintf(&hdrreq, "RFC822.HEADER.LINES (%s%s%s)", want_headers,
                  ImapHeaders ? " " : "", NONULL(ImapHeaders));
  }
  else
  {  
    mutt_error(_("Unable to fetch headers from this IMAP server version."));
    goto error_out_0;
  }

   
  mutt_mktemp(tempfile, sizeof(tempfile));
  fp = mutt_file_fopen(tempfile, "w+");
  if (!fp)
  {
    mutt_error(_("Could not create temporary file %s"), tempfile);
    goto error_out_0;
  }
  unlink(tempfile);

   
  while (msn_end > ctx->hdrmax)
    mx_alloc_memory(ctx);
  alloc_msn_index(idata, msn_end);

  idx = ctx->msgcount;
  oldmsgcount = ctx->msgcount;
  idata->reopen &= ~(IMAP_REOPEN_ALLOW | IMAP_NEWMAIL_PENDING);
  idata->new_mail_count = 0;

#ifdef USE_HCACHE
  idata->hcache = imap_hcache_open(idata, NULL);

  if (idata->hcache && (msn_begin == 1))
  {
    uid_validity = mutt_hcache_fetch_raw(idata->hcache, "/UIDVALIDITY", 12);
    puidnext = mutt_hcache_fetch_raw(idata->hcache, "/UIDNEXT", 8);
    if (puidnext)
    {
      uidnext = *(unsigned int *) puidnext;
      mutt_hcache_free(idata->hcache, &puidnext);
    }
    if (uid_validity && uidnext && *(unsigned int *) uid_validity == idata->uid_validity)
      evalhc = true;
    mutt_hcache_free(idata->hcache, &uid_validity);
  }
  if (evalhc)
  {
     
    mutt_progress_init(&progress, _("Evaluating cache..."), MUTT_PROGRESS_MSG,
                       ReadInc, msn_end);

    snprintf(buf, sizeof(buf), "UID FETCH 1:%u (UID FLAGS)", uidnext - 1);

    imap_cmd_start(idata, buf);

    rc = IMAP_CMD_CONTINUE;
    for (msgno = 1; rc == IMAP_CMD_CONTINUE; msgno++)
    {
      mutt_progress_update(&progress, msgno, -1);

      memset(&h, 0, sizeof(h));
      h.data = new_header_data();
      do
      {
        rc = imap_cmd_step(idata);
        if (rc != IMAP_CMD_CONTINUE)
          break;

        mfhrc = msg_fetch_header(ctx, &h, idata->buf, NULL);
        if (mfhrc < 0)
          continue;

        if (!h.data->uid)
        {
          mutt_debug(2,
                     "skipping hcache FETCH response for message number %d "
                     "missing a UID\n",
                     h.data->msn);
          continue;
        }

        if (h.data->msn < 1 || h.data->msn > msn_end)
        {
          mutt_debug(1, "skipping hcache FETCH response for unknown message number %d\n",
                     h.data->msn);
          continue;
        }

        if (idata->msn_index[h.data->msn - 1])
        {
          mutt_debug(2, "skipping hcache FETCH for duplicate message %d\n",
                     h.data->msn);
          continue;
        }

        ctx->hdrs[idx] = imap_hcache_get(idata, h.data->uid);
        if (ctx->hdrs[idx])
        {
          idata->max_msn = MAX(idata->max_msn, h.data->msn);
          idata->msn_index[h.data->msn - 1] = ctx->hdrs[idx];

          ctx->hdrs[idx]->index = idx;
           
          ctx->hdrs[idx]->active = true;
          ctx->hdrs[idx]->read = h.data->read;
          ctx->hdrs[idx]->old = h.data->old;
          ctx->hdrs[idx]->deleted = h.data->deleted;
          ctx->hdrs[idx]->flagged = h.data->flagged;
          ctx->hdrs[idx]->replied = h.data->replied;
          ctx->hdrs[idx]->changed = h.data->changed;
           
          ctx->hdrs[idx]->data = (void *) (h.data);
          STAILQ_INIT(&ctx->hdrs[idx]->tags);
          driver_tags_replace(&ctx->hdrs[idx]->tags, mutt_str_strdup(h.data->flags_remote));

          ctx->msgcount++;
          ctx->size += ctx->hdrs[idx]->content->length;

          h.data = NULL;
          idx++;
        }
      } while (mfhrc == -1);

      imap_free_header_data(&h.data);

      if ((mfhrc < -1) || ((rc != IMAP_CMD_CONTINUE) && (rc != IMAP_CMD_OK)))
      {
        imap_hcache_close(idata);
        goto error_out_1;
      }
    }

     
    while (msn_begin <= msn_end)
    {
      if (!idata->msn_index[msn_begin - 1])
        break;
      msn_begin++;
    }
  }
#endif  

  mutt_progress_init(&progress, _("Fetching message headers..."),
                     MUTT_PROGRESS_MSG, ReadInc, msn_end);

  while (msn_begin <= msn_end && fetch_msn_end < msn_end)
  {
    struct Buffer *b = mutt_buffer_new();
    if (evalhc)
    {
       
      evalhc = false;
      generate_seqset(b, idata, msn_begin, msn_end);
    }
    else
      mutt_buffer_printf(b, "%u:%u", msn_begin, msn_end);

    fetch_msn_end = msn_end;
    char *cmd = NULL;
    safe_asprintf(&cmd, "FETCH %s (UID FLAGS INTERNALDATE RFC822.SIZE %s)", b->data, hdrreq);
    imap_cmd_start(idata, cmd);
    FREE(&cmd);
    mutt_buffer_free(&b);

    rc = IMAP_CMD_CONTINUE;
    for (msgno = msn_begin; rc == IMAP_CMD_CONTINUE; msgno++)
    {
      mutt_progress_update(&progress, msgno, -1);

      rewind(fp);
      memset(&h, 0, sizeof(h));
      h.data = new_header_data();

       
      do
      {
        rc = imap_cmd_step(idata);
        if (rc != IMAP_CMD_CONTINUE)
          break;

        mfhrc = msg_fetch_header(ctx, &h, idata->buf, fp);
        if (mfhrc < 0)
          continue;

        if (!ftello(fp))
        {
          mutt_debug(
              2, "msg_fetch_header: ignoring fetch response with no body\n");
          continue;
        }

         
        fputs("\n\n", fp);

        if (h.data->msn < 1 || h.data->msn > fetch_msn_end)
        {
          mutt_debug(1, "skipping FETCH response for unknown message number %d\n",
                     h.data->msn);
          continue;
        }

         
        if (idata->msn_index[h.data->msn - 1])
        {
          mutt_debug(2, "skipping FETCH response for duplicate message %d\n",
                     h.data->msn);
          continue;
        }

        ctx->hdrs[idx] = mutt_header_new();

        idata->max_msn = MAX(idata->max_msn, h.data->msn);
        idata->msn_index[h.data->msn - 1] = ctx->hdrs[idx];

        ctx->hdrs[idx]->index = idx;
         
        ctx->hdrs[idx]->active = true;
        ctx->hdrs[idx]->read = h.data->read;
        ctx->hdrs[idx]->old = h.data->old;
        ctx->hdrs[idx]->deleted = h.data->deleted;
        ctx->hdrs[idx]->flagged = h.data->flagged;
        ctx->hdrs[idx]->replied = h.data->replied;
        ctx->hdrs[idx]->changed = h.data->changed;
        ctx->hdrs[idx]->received = h.received;
        ctx->hdrs[idx]->data = (void *) (h.data);
        STAILQ_INIT(&ctx->hdrs[idx]->tags);
        driver_tags_replace(&ctx->hdrs[idx]->tags, mutt_str_strdup(h.data->flags_remote));

        if (maxuid < h.data->uid)
          maxuid = h.data->uid;

        rewind(fp);
         
        ctx->hdrs[idx]->env = mutt_rfc822_read_header(fp, ctx->hdrs[idx], 0, 0);
         
        ctx->hdrs[idx]->content->length = h.content_length;
        ctx->size += h.content_length;

#ifdef USE_HCACHE
        imap_hcache_put(idata, ctx->hdrs[idx]);
#endif  

        ctx->msgcount++;

        h.data = NULL;
        idx++;
      } while (mfhrc == -1);

      imap_free_header_data(&h.data);

      if ((mfhrc < -1) || ((rc != IMAP_CMD_CONTINUE) && (rc != IMAP_CMD_OK)))
      {
#ifdef USE_HCACHE
        imap_hcache_close(idata);
#endif
        goto error_out_1;
      }
    }

     
    if (idata->reopen & IMAP_NEWMAIL_PENDING)
    {
       
      fetch_msn_end = idata->max_msn;
      msn_begin = idata->max_msn + 1;
      msn_end = idata->new_mail_count;
      while (msn_end > ctx->hdrmax)
        mx_alloc_memory(ctx);
      alloc_msn_index(idata, msn_end);
      idata->reopen &= ~IMAP_NEWMAIL_PENDING;
      idata->new_mail_count = 0;
    }
  }

  if (maxuid && (status = imap_mboxcache_get(idata, idata->mailbox, 0)) &&
      (status->uidnext < maxuid + 1))
  {
    status->uidnext = maxuid + 1;
  }

#ifdef USE_HCACHE
  mutt_hcache_store_raw(idata->hcache, "/UIDVALIDITY", 12, &idata->uid_validity,
                        sizeof(idata->uid_validity));
  if (maxuid && idata->uidnext < maxuid + 1)
  {
    mutt_debug(2, "Overriding UIDNEXT: %u -> %u\n", idata->uidnext, maxuid + 1);
    idata->uidnext = maxuid + 1;
  }
  if (idata->uidnext > 1)
  {
    mutt_hcache_store_raw(idata->hcache, "/UIDNEXT", 8, &idata->uidnext,
                          sizeof(idata->uidnext));
  }

  imap_hcache_close(idata);
#endif  

  if (ctx->msgcount > oldmsgcount)
  {
     
    mx_alloc_memory(ctx);
    mx_update_context(ctx, ctx->msgcount - oldmsgcount);
    update_context(idata, oldmsgcount);
  }

  idata->reopen |= IMAP_REOPEN_ALLOW;

  retval = msn_end;

error_out_1:
  mutt_file_fclose(&fp);

error_out_0:
  FREE(&hdrreq);

  return retval;
}
