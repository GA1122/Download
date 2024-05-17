read_number(int *n, uschar *s)
{
*n = 0;
while (isdigit(*s)) *n = *n * 10 + (*s++ - '0');
return s;
}
