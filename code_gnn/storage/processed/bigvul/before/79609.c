static int imap_tags_commit(struct Context *ctx, struct Header *hdr, char *buf)
{
  struct Buffer *cmd = NULL;
  char uid[11];

  struct ImapData *idata = ctx->data;

  if (*buf == '\0')
    buf = NULL;

  if (!mutt_bit_isset(idata->ctx->rights, MUTT_ACL_WRITE))
    return 0;

  snprintf(uid, sizeof(uid), "%u", HEADER_DATA(hdr)->uid);

   
  if (HEADER_DATA(hdr)->flags_remote)
  {
    cmd = mutt_buffer_new();
    if (!cmd)
    {
      mutt_debug(1, "unable to allocate buffer\n");
      return -1;
    }
    cmd->dptr = cmd->data;
    mutt_buffer_addstr(cmd, "UID STORE ");
    mutt_buffer_addstr(cmd, uid);
    mutt_buffer_addstr(cmd, " -FLAGS.SILENT (");
    mutt_buffer_addstr(cmd, HEADER_DATA(hdr)->flags_remote);
    mutt_buffer_addstr(cmd, ")");

     
    if (imap_exec(idata, cmd->data, 0) != 0)
    {
      mutt_buffer_free(&cmd);
      return -1;
    }

    mutt_buffer_free(&cmd);
  }

   
  if (buf)
  {
    cmd = mutt_buffer_new();
    if (!cmd)
    {
      mutt_debug(1, "fail to remove old flags\n");
      return -1;
    }
    cmd->dptr = cmd->data;
    mutt_buffer_addstr(cmd, "UID STORE ");
    mutt_buffer_addstr(cmd, uid);
    mutt_buffer_addstr(cmd, " +FLAGS.SILENT (");
    mutt_buffer_addstr(cmd, buf);
    mutt_buffer_addstr(cmd, ")");

    if (imap_exec(idata, cmd->data, 0) != 0)
    {
      mutt_debug(1, "fail to add new flags\n");
      mutt_buffer_free(&cmd);
      return -1;
    }

    mutt_buffer_free(&cmd);
  }

   
  mutt_debug(1, "NEW TAGS: %d\n", buf);
  driver_tags_replace(&hdr->tags, buf);
  FREE(&HEADER_DATA(hdr)->flags_remote);
  HEADER_DATA(hdr)->flags_remote = driver_tags_get_with_hidden(&hdr->tags);
  return 0;
}