extract_substr(uschar *subject, int value1, int value2, int *len)
{
int sublen = Ustrlen(subject);

if (value1 < 0)     
  {
  value1 += sublen;

   

  if (value1 < 0)
    {
    value2 += value1;
    if (value2 < 0) value2 = 0;
    value1 = 0;
    }

   

  else if (value2 < 0)
    {
    value2 = value1;
    value1 = 0;
    }
  }

 

else
  {
  if (value1 > sublen)
    {
    value1 = sublen;
    value2 = 0;
    }
  else if (value2 < 0) value2 = sublen;
  }

 

if (value1 + value2 > sublen) value2 = sublen - value1;
*len = value2;
return subject + value1;
}
