static char *get_flags(struct ListHead *hflags, char *s)
{
   
  if (mutt_str_strncasecmp("FLAGS", s, 5) != 0)
  {
    mutt_debug(1, "not a FLAGS response: %s\n", s);
    return NULL;
  }
  s += 5;
  SKIPWS(s);
  if (*s != '(')
  {
    mutt_debug(1, "bogus FLAGS response: %s\n", s);
    return NULL;
  }

   
  while (*s && *s != ')')
  {
    s++;
    SKIPWS(s);
    const char *flag_word = s;
    while (*s && (*s != ')') && !ISSPACE(*s))
      s++;
    const char ctmp = *s;
    *s = '\0';
    if (*flag_word)
      mutt_list_insert_tail(hflags, mutt_str_strdup(flag_word));
    *s = ctmp;
  }

   
  if (*s != ')')
  {
    mutt_debug(1, "Unterminated FLAGS response: %s\n", s);
    mutt_list_free(hflags);

    return NULL;
  }

  s++;

  return s;
}