expand_string(uschar *string)
{
search_find_defer = FALSE;
malformed_header = FALSE;
return (Ustrpbrk(string, "$\\") == NULL)? string :
  expand_string_internal(string, FALSE, NULL, FALSE, TRUE, NULL);
}
