eval_op_mult(uschar **sptr, BOOL decimal, uschar **error)
{
uschar *s = *sptr;
int_eximarith_t x = eval_op_unary(&s, decimal, error);
if (*error == NULL)
  {
  while (*s == '*' || *s == '/' || *s == '%')
    {
    int op = *s++;
    int_eximarith_t y = eval_op_unary(&s, decimal, error);
    if (*error != NULL) break;
     
    if (y == -1 && x == EXIM_ARITH_MIN && op != '*')
      {
      DEBUG(D_expand)
        debug_printf("Integer exception dodging: " PR_EXIM_ARITH "%c-1 coerced to " PR_EXIM_ARITH "\n",
            EXIM_ARITH_MIN, op, EXIM_ARITH_MAX);
      x = EXIM_ARITH_MAX;
      continue;
      }
    if (op == '*')
      x *= y;
    else
      {
      if (y == 0)
        {
        *error = (op == '/') ? US"divide by zero" : US"modulo by zero";
        x = 0;
        break;
        }
      if (op == '/')
        x /= y;
      else
        x %= y;
      }
    }
  }
*sptr = s;
return x;
}
