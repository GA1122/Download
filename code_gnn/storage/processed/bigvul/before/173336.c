uarb_shift(uarb inout, int ndigits, unsigned int right_shift)
  
{
   FIX_GCC int i = ndigits;
   png_uint_16 carry = 0;

   assert(right_shift >= 1 && right_shift <= 15);

 while (--i >= 0)
 {
      png_uint_16 temp = (png_uint_16)(carry | (inout[i] >> right_shift));

  
      carry = (png_uint_16)((inout[i] << (16-right_shift)) & 0xffff);

      inout[i] = temp;

  
 if (i == ndigits-1 && temp == 0)
         ndigits = i;
 }

 return ndigits;
}
