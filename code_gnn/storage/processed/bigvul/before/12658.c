expand_gettokened (int field, uschar *separators, uschar *s)
{
int sep = 1;
int count;
uschar *ss = s;
uschar *fieldtext = NULL;

if (field == 0) return s;

 

count = (field > 0)? field : INT_MAX;

while (count-- > 0)
  {
  size_t len;

   

  if (sep == 0)
    {
    if (field > 0 || (-field) > (INT_MAX - count - 1)) return NULL;
    if ((-field) == (INT_MAX - count - 1)) return s;
    while (field++ < 0)
      {
      ss--;
      while (ss[-1] != 0) ss--;
      }
    fieldtext = ss;
    break;
    }

   

  fieldtext = ss;
  len = Ustrcspn(ss, separators);
  sep = ss[len];
  ss[len] = 0;
  ss += len + 1;
  }

return fieldtext;
}
