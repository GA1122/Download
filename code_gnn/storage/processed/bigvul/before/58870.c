macros_trusted(BOOL opt_D_used)
{
#ifdef WHITELIST_D_MACROS
macro_item *m;
uschar *whitelisted, *end, *p, **whites, **w;
int white_count, i, n;
size_t len;
BOOL prev_char_item, found;
#endif

if (!opt_D_used)
  return TRUE;
#ifndef WHITELIST_D_MACROS
return FALSE;
#else

 
if ( ! ((real_uid == root_uid)
     || (real_uid == exim_uid)
#ifdef CONFIGURE_OWNER
     || (real_uid == config_uid)
#endif
   ))
  {
  debug_printf("macros_trusted rejecting macros for uid %d\n", (int) real_uid);
  return FALSE;
  }

 
whitelisted = string_copy_malloc(US WHITELIST_D_MACROS);
prev_char_item = FALSE;
white_count = 0;
for (p = whitelisted; *p != '\0'; ++p)
  {
  if (*p == ':' || isspace(*p))
    {
    *p = '\0';
    if (prev_char_item)
      ++white_count;
    prev_char_item = FALSE;
    continue;
    }
  if (!prev_char_item)
    prev_char_item = TRUE;
  }
end = p;
if (prev_char_item)
  ++white_count;
if (!white_count)
  return FALSE;
whites = store_malloc(sizeof(uschar *) * (white_count+1));
for (p = whitelisted, i = 0; (p != end) && (i < white_count); ++p)
  {
  if (*p != '\0')
    {
    whites[i++] = p;
    if (i == white_count)
      break;
    while (*p != '\0' && p < end)
      ++p;
    }
  }
whites[i] = NULL;

 
for (m = macros; m; m = m->next) if (m->command_line)
  {
  found = FALSE;
  for (w = whites; *w; ++w)
    if (Ustrcmp(*w, m->name) == 0)
      {
      found = TRUE;
      break;
      }
  if (!found)
    return FALSE;
  if (m->replacement == NULL)
    continue;
  len = Ustrlen(m->replacement);
  if (len == 0)
    continue;
  n = pcre_exec(regex_whitelisted_macro, NULL, CS m->replacement, len,
   0, PCRE_EOPT, NULL, 0);
  if (n < 0)
    {
    if (n != PCRE_ERROR_NOMATCH)
      debug_printf("macros_trusted checking %s returned %d\n", m->name, n);
    return FALSE;
    }
  }
DEBUG(D_any) debug_printf("macros_trusted overridden to true by whitelisting\n");
return TRUE;
#endif
}