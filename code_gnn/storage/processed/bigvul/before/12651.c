eval_op_shift(uschar **sptr, BOOL decimal, uschar **error)
{
uschar *s = *sptr;
int_eximarith_t x = eval_op_sum(&s, decimal, error);
if (*error == NULL)
  {
  while ((*s == '<' || *s == '>') && s[1] == s[0])
    {
    int_eximarith_t y;
    int op = *s++;
    s++;
    y = eval_op_sum(&s, decimal, error);
    if (*error != NULL) break;
    if (op == '<') x <<= y; else x >>= y;
    }
  }
*sptr = s;
return x;
}
