compute_hash(uschar *subject, int value1, int value2, int *len)
{
int sublen = Ustrlen(subject);

if (value2 < 0) value2 = 26;
else if (value2 > Ustrlen(hashcodes))
  {
  expand_string_message =
    string_sprintf("hash count \"%d\" too big", value2);
  return NULL;
  }

 

if (value1 < sublen)
  {
  int c;
  int i = 0;
  int j = value1;
  while ((c = (subject[j])) != 0)
    {
    int shift = (c + j++) & 7;
    subject[i] ^= (c << shift) | (c >> (8-shift));
    if (++i >= value1) i = 0;
    }
  for (i = 0; i < value1; i++)
    subject[i] = hashcodes[(subject[i]) % value2];
  }
else value1 = sublen;

*len = value1;
return subject;
}
