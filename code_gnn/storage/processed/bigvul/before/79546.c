static int msg_fetch_header(struct Context *ctx, struct ImapHeader *h, char *buf, FILE *fp)
{
  unsigned int bytes;
  int rc = -1;  
  int parse_rc;

  struct ImapData *idata = ctx->data;

  if (buf[0] != '*')
    return rc;

   
  buf = imap_next_word(buf);
  if (mutt_str_atoui(buf, &h->data->msn) < 0)
    return rc;

   
  buf = imap_next_word(buf);
  if (mutt_str_strncasecmp("FETCH", buf, 5) != 0)
    return rc;

  rc = -2;  
  buf = strchr(buf, '(');
  if (!buf)
    return rc;
  buf++;

   
  parse_rc = msg_parse_fetch(h, buf);
  if (!parse_rc)
    return 0;
  if (parse_rc != -2 || !fp)
    return rc;

  if (imap_get_literal_count(buf, &bytes) == 0)
  {
    imap_read_literal(fp, idata, bytes, NULL);

     
    if (imap_cmd_step(idata) != IMAP_CMD_CONTINUE)
      return rc;

    if (msg_parse_fetch(h, idata->buf) == -1)
      return rc;
  }

  rc = 0;  

   
  h->content_length -= bytes;

  return rc;
}
