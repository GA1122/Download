static void cmd_parse_fetch(struct ImapData *idata, char *s)
{
  unsigned int msn, uid;
  struct Header *h = NULL;
  int server_changes = 0;

  mutt_debug(3, "Handling FETCH\n");

  if (mutt_str_atoui(s, &msn) < 0 || msn < 1 || msn > idata->max_msn)
  {
    mutt_debug(3, "#1 FETCH response ignored for this message\n");
    return;
  }

  h = idata->msn_index[msn - 1];
  if (!h || !h->active)
  {
    mutt_debug(3, "#2 FETCH response ignored for this message\n");
    return;
  }

  mutt_debug(2, "Message UID %u updated\n", HEADER_DATA(h)->uid);
   
  s = imap_next_word(s);
  s = imap_next_word(s);

  if (*s != '(')
  {
    mutt_debug(1, "Malformed FETCH response\n");
    return;
  }
  s++;

  while (*s)
  {
    SKIPWS(s);

    if (mutt_str_strncasecmp("FLAGS", s, 5) == 0)
    {
      imap_set_flags(idata, h, s, &server_changes);
      if (server_changes)
      {
         
        if (h->changed)
          idata->reopen |= IMAP_EXPUNGE_PENDING;
        else
          idata->check_status = IMAP_FLAGS_PENDING;
      }
      return;
    }
    else if (mutt_str_strncasecmp("UID", s, 3) == 0)
    {
      s += 3;
      SKIPWS(s);
      if (mutt_str_atoui(s, &uid) < 0)
      {
        mutt_debug(2, "Illegal UID.  Skipping update.\n");
        return;
      }
      if (uid != HEADER_DATA(h)->uid)
      {
        mutt_debug(2, "FETCH UID vs MSN mismatch.  Skipping update.\n");
        return;
      }
      s = imap_next_word(s);
    }
    else if (*s == ')')
      s++;  
    else if (*s)
    {
      mutt_debug(2, "Only handle FLAGS updates\n");
      return;
    }
  }
}