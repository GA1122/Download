compute_nhash (uschar *subject, int value1, int value2, int *len)
{
uschar *s = subject;
int i = 0;
unsigned long int total = 0;  

while (*s != 0)
  {
  if (i == 0) i = sizeof(prime)/sizeof(int) - 1;
  total += prime[i--] * (unsigned int)(*s++);
  }

 

if (value2 < 0)
  {
  s = string_sprintf("%d", total % value1);
  }

 

else
  {
  total = total % (value1 * value2);
  s = string_sprintf("%d/%d", total/value2, total % value2);
  }

*len = Ustrlen(s);
return s;
}
