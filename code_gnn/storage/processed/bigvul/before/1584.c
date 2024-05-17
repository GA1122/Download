aspath_gettoken (const char *buf, enum as_token *token, u_long *asno)
{
  const char *p = buf;

   
  while (isspace ((int) *p) || *p == ',')
    p++;

   
  switch (*p)
    {
    case '\0':
      return NULL;
    case '{':
      *token = as_token_set_start;
      p++;
      return p;
    case '}':
      *token = as_token_set_end;
      p++;
      return p;
    case '(':
      *token = as_token_confed_seq_start;
      p++;
      return p;
    case ')':
      *token = as_token_confed_seq_end;
      p++;
      return p;
    case '[':
      *token = as_token_confed_set_start;
      p++;
      return p;
    case ']':
      *token = as_token_confed_set_end;
      p++;
      return p;
    }

   
  if (isdigit ((int) *p)) 
    {
      as_t asval;
      
      *token = as_token_asval;
      asval = (*p - '0');
      p++;
      
      while (isdigit ((int) *p)) 
        {
          asval *= 10;
          asval += (*p - '0');
          p++;
        }
      *asno = asval;
      return p;
    }
  
   
  *token = as_token_unknown;
  return  p++;
}