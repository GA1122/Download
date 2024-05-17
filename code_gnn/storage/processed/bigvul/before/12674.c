read_subs(uschar **sub, int n, int m, uschar **sptr, BOOL skipping,
  BOOL check_end, uschar *name, BOOL *resetok)
{
int i;
uschar *s = *sptr;

while (isspace(*s)) s++;
for (i = 0; i < n; i++)
  {
  if (*s != '{')
    {
    if (i < m) return 1;
    sub[i] = NULL;
    break;
    }
  sub[i] = expand_string_internal(s+1, TRUE, &s, skipping, TRUE, resetok);
  if (sub[i] == NULL) return 3;
  if (*s++ != '}') return 1;
  while (isspace(*s)) s++;
  }
if (check_end && *s++ != '}')
