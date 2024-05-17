static int imap_tags_edit(struct Context *ctx, const char *tags, char *buf, size_t buflen)
{
  char *new = NULL;
  char *checker = NULL;
  struct ImapData *idata = (struct ImapData *) ctx->data;

   
  if (!imap_has_flag(&idata->flags, NULL))
  {
    mutt_error(_("IMAP server doesn't support custom flags"));
    return -1;
  }

  *buf = '\0';
  if (tags)
    strncpy(buf, tags, buflen);

  if (mutt_get_field("Tags: ", buf, buflen, 0) != 0)
    return -1;

   

  new = buf;
  checker = buf;
  SKIPWS(checker);
  while (*checker != '\0')
  {
    if (*checker < 32 || *checker >= 127 ||  
        *checker == 40 ||                    
        *checker == 41 ||                    
        *checker == 60 ||                    
        *checker == 62 ||                    
        *checker == 64 ||                    
        *checker == 44 ||                    
        *checker == 59 ||                    
        *checker == 58 ||                    
        *checker == 92 ||                    
        *checker == 34 ||                    
        *checker == 46 ||                    
        *checker == 91 ||                    
        *checker == 93)                      
    {
      mutt_error(_("Invalid IMAP flags"));
      return 0;
    }

     
    while (*checker == ' ' && *(checker + 1) == ' ')
      checker++;

     
    *new ++ = *checker++;
  }
  *new = '\0';
  new = buf;  
  mutt_str_remove_trailing_ws(new);

  if (mutt_str_strcmp(tags, buf) == 0)
    return 0;
  return 1;
}
