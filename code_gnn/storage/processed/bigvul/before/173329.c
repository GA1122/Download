uarb_copy(uarb to, uarb from, int idigits)
  
{
 int d, odigits;

 for (d=odigits=0; d<idigits; ++d)
 if ((to[d] = from[d]) != 0)
         odigits = d+1;

 return odigits;
}
