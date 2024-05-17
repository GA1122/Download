int imap_complete(char *buf, size_t buflen, char *path)
{
  struct ImapData *idata = NULL;
  char list[LONG_STRING];
  char tmp[LONG_STRING];
  struct ImapList listresp;
  char completion[LONG_STRING];
  int clen;
  size_t matchlen = 0;
  int completions = 0;
  struct ImapMbox mx;
  int rc;

  if (imap_parse_path(path, &mx))
  {
    mutt_str_strfcpy(buf, path, buflen);
    return complete_hosts(buf, buflen);
  }

   
  idata = imap_conn_find(&(mx.account), MUTT_IMAP_CONN_NONEW);
  if (!idata)
  {
    FREE(&mx.mbox);
    mutt_str_strfcpy(buf, path, buflen);
    return complete_hosts(buf, buflen);
  }

   
   
  if (mx.mbox && mx.mbox[0])
    imap_fix_path(idata, mx.mbox, list, sizeof(list));
  else
    list[0] = '\0';

   
  snprintf(tmp, sizeof(tmp), "%s \"\" \"%s%%\"", ImapListSubscribed ? "LSUB" : "LIST", list);

  imap_cmd_start(idata, tmp);

   
  mutt_str_strfcpy(completion, NONULL(mx.mbox), sizeof(completion));
  idata->cmdtype = IMAP_CT_LIST;
  idata->cmddata = &listresp;
  do
  {
    listresp.name = NULL;
    rc = imap_cmd_step(idata);

    if (rc == IMAP_CMD_CONTINUE && listresp.name)
    {
       
      if (listresp.noselect)
      {
        clen = strlen(listresp.name);
        listresp.name[clen++] = listresp.delim;
        listresp.name[clen] = '\0';
      }
       
      if (!completions)
      {
        mutt_str_strfcpy(completion, listresp.name, sizeof(completion));
        matchlen = strlen(completion);
        completions++;
        continue;
      }

      matchlen = longest_common_prefix(completion, listresp.name, 0, matchlen);
      completions++;
    }
  } while (rc == IMAP_CMD_CONTINUE);
  idata->cmddata = NULL;

  if (completions)
  {
     
    imap_qualify_path(buf, buflen, &mx, completion);
    mutt_pretty_mailbox(buf, buflen);

    FREE(&mx.mbox);
    return 0;
  }

  return -1;
}