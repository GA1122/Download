int imap_sync_message_for_copy(struct ImapData *idata, struct Header *hdr,
                               struct Buffer *cmd, int *err_continue)
{
  char flags[LONG_STRING];
  char *tags;
  char uid[11];

  if (!compare_flags_for_copy(hdr))
  {
    if (hdr->deleted == HEADER_DATA(hdr)->deleted)
      hdr->changed = false;
    return 0;
  }

  snprintf(uid, sizeof(uid), "%u", HEADER_DATA(hdr)->uid);
  cmd->dptr = cmd->data;
  mutt_buffer_addstr(cmd, "UID STORE ");
  mutt_buffer_addstr(cmd, uid);

  flags[0] = '\0';

  set_flag(idata, MUTT_ACL_SEEN, hdr->read, "\\Seen ", flags, sizeof(flags));
  set_flag(idata, MUTT_ACL_WRITE, hdr->old, "Old ", flags, sizeof(flags));
  set_flag(idata, MUTT_ACL_WRITE, hdr->flagged, "\\Flagged ", flags, sizeof(flags));
  set_flag(idata, MUTT_ACL_WRITE, hdr->replied, "\\Answered ", flags, sizeof(flags));
  set_flag(idata, MUTT_ACL_DELETE, HEADER_DATA(hdr)->deleted, "\\Deleted ",
           flags, sizeof(flags));

  if (mutt_bit_isset(idata->ctx->rights, MUTT_ACL_WRITE))
  {
     
    if (HEADER_DATA(hdr)->flags_system)
      mutt_str_strcat(flags, sizeof(flags), HEADER_DATA(hdr)->flags_system);
     
    tags = driver_tags_get_with_hidden(&hdr->tags);
    if (tags)
    {
      mutt_str_strcat(flags, sizeof(flags), tags);
      FREE(&tags);
    }
  }

  mutt_str_remove_trailing_ws(flags);

   
  if (!*flags)
  {
    set_flag(idata, MUTT_ACL_SEEN, 1, "\\Seen ", flags, sizeof(flags));
    set_flag(idata, MUTT_ACL_WRITE, 1, "Old ", flags, sizeof(flags));
    set_flag(idata, MUTT_ACL_WRITE, 1, "\\Flagged ", flags, sizeof(flags));
    set_flag(idata, MUTT_ACL_WRITE, 1, "\\Answered ", flags, sizeof(flags));
    set_flag(idata, MUTT_ACL_DELETE, !HEADER_DATA(hdr)->deleted, "\\Deleted ",
             flags, sizeof(flags));

     
    if (mutt_bit_isset(idata->ctx->rights, MUTT_ACL_WRITE) && HEADER_DATA(hdr)->flags_remote)
      mutt_str_strcat(flags, sizeof(flags), HEADER_DATA(hdr)->flags_remote);

    mutt_str_remove_trailing_ws(flags);

    mutt_buffer_addstr(cmd, " -FLAGS.SILENT (");
  }
  else
    mutt_buffer_addstr(cmd, " FLAGS.SILENT (");

  mutt_buffer_addstr(cmd, flags);
  mutt_buffer_addstr(cmd, ")");

   
  hdr->active = false;

   
  if (*flags && (imap_exec(idata, cmd->data, 0) != 0) && err_continue &&
      (*err_continue != MUTT_YES))
  {
    *err_continue = imap_continue("imap_sync_message: STORE failed", idata->buf);
    if (*err_continue != MUTT_YES)
    {
      hdr->active = true;
      return -1;
    }
  }

   
  FREE(&HEADER_DATA(hdr)->flags_remote);
  HEADER_DATA(hdr)->flags_remote = driver_tags_get_with_hidden(&hdr->tags);

  hdr->active = true;
  if (hdr->deleted == HEADER_DATA(hdr)->deleted)
    hdr->changed = false;

  return 0;
}