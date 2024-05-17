int imap_cmd_step(struct ImapData *idata)
{
  size_t len = 0;
  int c;
  int rc;
  int stillrunning = 0;
  struct ImapCommand *cmd = NULL;

  if (idata->status == IMAP_FATAL)
  {
    cmd_handle_fatal(idata);
    return IMAP_CMD_BAD;
  }

   
  do
  {
    if (len == idata->blen)
    {
      mutt_mem_realloc(&idata->buf, idata->blen + IMAP_CMD_BUFSIZE);
      idata->blen = idata->blen + IMAP_CMD_BUFSIZE;
      mutt_debug(3, "grew buffer to %u bytes\n", idata->blen);
    }

     
    if (len)
      len--;
    c = mutt_socket_readln(idata->buf + len, idata->blen - len, idata->conn);
    if (c <= 0)
    {
      mutt_debug(1, "Error reading server response.\n");
      cmd_handle_fatal(idata);
      return IMAP_CMD_BAD;
    }

    len += c;
  }
   
  while (len == idata->blen);

   
  if ((idata->blen > IMAP_CMD_BUFSIZE) && (len <= IMAP_CMD_BUFSIZE))
  {
    mutt_mem_realloc(&idata->buf, IMAP_CMD_BUFSIZE);
    idata->blen = IMAP_CMD_BUFSIZE;
    mutt_debug(3, "shrank buffer to %u bytes\n", idata->blen);
  }

  idata->lastread = time(NULL);

   
  if (((mutt_str_strncmp(idata->buf, "* ", 2) == 0) ||
       (mutt_str_strncmp(imap_next_word(idata->buf), "OK [", 4) == 0)) &&
      cmd_handle_untagged(idata))
  {
    return IMAP_CMD_BAD;
  }

   
  if (idata->buf[0] == '+')
    return IMAP_CMD_RESPOND;

   
  rc = IMAP_CMD_OK;
  c = idata->lastcmd;
  do
  {
    cmd = &idata->cmds[c];
    if (cmd->state == IMAP_CMD_NEW)
    {
      if (mutt_str_strncmp(idata->buf, cmd->seq, SEQLEN) == 0)
      {
        if (!stillrunning)
        {
           
          idata->lastcmd = (idata->lastcmd + 1) % idata->cmdslots;
        }
        cmd->state = cmd_status(idata->buf);
         
        rc = cmd->state;
      }
      else
        stillrunning++;
    }

    c = (c + 1) % idata->cmdslots;
  } while (c != idata->nextcmd);

  if (stillrunning)
    rc = IMAP_CMD_CONTINUE;
  else
  {
    mutt_debug(3, "IMAP queue drained\n");
    imap_cmd_finish(idata);
  }

  return rc;
}
