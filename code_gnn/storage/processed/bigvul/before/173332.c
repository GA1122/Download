uarb_mult_digit(uarb acc, int a_digits, uarb num, FIX_GCC int n_digits,
   png_uint_16 val)
  
{
  
 if (val > 0 && n_digits > 0)  
 {
      png_uint_32 carry = 0;
 int out_digits = 0;

 while (out_digits < n_digits || carry > 0)
 {
 if (out_digits < a_digits)
            carry += acc[out_digits];

 if (out_digits < n_digits)
            carry += (png_uint_32)num[out_digits] * val;

         acc[out_digits++] = (png_uint_16)(carry & 0xffff);
         carry >>= 16;
 }

  
 if (out_digits > a_digits)
 return out_digits;
 }

 return a_digits;
}
