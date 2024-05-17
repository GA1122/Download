uarb_cmp(uarb a, int adigits, uarb b, int bdigits)
  
{
 if (adigits < bdigits)
 return -1;

 if (adigits > bdigits)
 return 1;

 while (adigits-- > 0)
 if (a[adigits] < b[adigits])
 return -1;

 else if (a[adigits] > b[adigits])
 return 1;

 return 0;
}
