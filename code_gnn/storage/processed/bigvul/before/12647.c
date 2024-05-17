eval_number(uschar **sptr, BOOL decimal, uschar **error)
{
register int c;
int_eximarith_t n;
uschar *s = *sptr;
while (isspace(*s)) s++;
c = *s;
if (isdigit(c))
  {
  int count;
  (void)sscanf(CS s, (decimal? SC_EXIM_DEC "%n" : SC_EXIM_ARITH "%n"), &n, &count);
  s += count;
  switch (tolower(*s))
    {
    default: break;
    case 'k': n *= 1024; s++; break;
    case 'm': n *= 1024*1024; s++; break;
    case 'g': n *= 1024*1024*1024; s++; break;
    }
  while (isspace (*s)) s++;
  }
else if (c == '(')
  {
  s++;
  n = eval_expr(&s, decimal, error, 1);
  }
else
  {
  *error = US"expecting number or opening parenthesis";
  n = 0;
  }
*sptr = s;
return n;
}
