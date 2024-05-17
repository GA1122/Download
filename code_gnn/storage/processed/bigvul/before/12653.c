eval_op_unary(uschar **sptr, BOOL decimal, uschar **error)
{
uschar *s = *sptr;
int_eximarith_t x;
while (isspace(*s)) s++;
if (*s == '+' || *s == '-' || *s == '~')
  {
  int op = *s++;
  x = eval_op_unary(&s, decimal, error);
  if (op == '-') x = -x;
    else if (op == '~') x = ~x;
  }
else
  {
  x = eval_number(&s, decimal, error);
  }
*sptr = s;
return x;
}
