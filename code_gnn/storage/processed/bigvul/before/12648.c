eval_op_and(uschar **sptr, BOOL decimal, uschar **error)
{
uschar *s = *sptr;
int_eximarith_t x = eval_op_shift(&s, decimal, error);
if (*error == NULL)
  {
  while (*s == '&')
    {
    int_eximarith_t y;
    s++;
    y = eval_op_shift(&s, decimal, error);
    if (*error != NULL) break;
    x &= y;
    }
  }
*sptr = s;
return x;
}
