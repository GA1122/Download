uarb_print(uarb num, int digits, FILE *out)
  
{
 if (digits * sizeof (udigit) > sizeof (unsigned long))
      uarb_printx(num, digits, out);

 else
 {
 unsigned long n = 0;

 while (digits > 0)
         n = (n << 16) + num[--digits];

      fprintf(out, "%lu", n);
 }
}
