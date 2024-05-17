eval_op_sum(uschar **sptr, BOOL decimal, uschar **error)
{
uschar *s = *sptr;
int_eximarith_t x = eval_op_mult(&s, decimal, error);
if (*error == NULL)
  {
  while (*s == '+' || *s == '-')
    {
    int op = *s++;
    int_eximarith_t y = eval_op_mult(&s, decimal, error);
    if (*error != NULL) break;
    if (op == '+') x += y; else x -= y;
    }
  }
*sptr = s;
return x;
}
