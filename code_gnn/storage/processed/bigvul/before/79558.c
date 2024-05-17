static void cmd_parse_list(struct ImapData *idata, char *s)
{
  struct ImapList *list = NULL;
  struct ImapList lb;
  char delimbuf[5];  
  unsigned int litlen;

  if (idata->cmddata && idata->cmdtype == IMAP_CT_LIST)
    list = (struct ImapList *) idata->cmddata;
  else
    list = &lb;

  memset(list, 0, sizeof(struct ImapList));

   
  s = imap_next_word(s);
  if (*s != '(')
  {
    mutt_debug(1, "Bad LIST response\n");
    return;
  }
  s++;
  while (*s)
  {
    if (mutt_str_strncasecmp(s, "\\NoSelect", 9) == 0)
      list->noselect = true;
    else if (mutt_str_strncasecmp(s, "\\NoInferiors", 12) == 0)
      list->noinferiors = true;
     
    else if (mutt_str_strncasecmp(s, "\\HasNoChildren", 14) == 0)
      list->noinferiors = true;

    s = imap_next_word(s);
    if (*(s - 2) == ')')
      break;
  }

   
  if (mutt_str_strncasecmp(s, "NIL", 3) != 0)
  {
    delimbuf[0] = '\0';
    mutt_str_strcat(delimbuf, 5, s);
    imap_unquote_string(delimbuf);
    list->delim = delimbuf[0];
  }

   
  s = imap_next_word(s);
   
  if (imap_get_literal_count(s, &litlen) == 0)
  {
    if (imap_cmd_step(idata) != IMAP_CMD_CONTINUE)
    {
      idata->status = IMAP_FATAL;
      return;
    }
    list->name = idata->buf;
  }
  else
  {
    imap_unmunge_mbox_name(idata, s);
    list->name = s;
  }

  if (list->name[0] == '\0')
  {
    idata->delim = list->delim;
    mutt_debug(3, "Root delimiter: %c\n", idata->delim);
  }
}
