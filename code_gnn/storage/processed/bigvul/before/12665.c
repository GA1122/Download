fn_recipients(void)
{
if (!enable_dollar_recipients) return NULL; else
  {
  int size = 128;
  int ptr = 0;
  int i;
  uschar * s = store_get(size);
  for (i = 0; i < recipients_count; i++)
    {
    if (i != 0) s = string_cat(s, &size, &ptr, US", ", 2);
    s = string_cat(s, &size, &ptr, recipients_list[i].address,
      Ustrlen(recipients_list[i].address));
    }
  s[ptr] = 0;      
  return s;
  }
}
