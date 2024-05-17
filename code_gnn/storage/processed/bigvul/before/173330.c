uarb_inc(uarb num, int in_digits, png_int_32 add)
  
{
   FIX_GCC int out_digits = 0;

 while (out_digits < in_digits)
 {
      add += num[out_digits];
      num[out_digits++] = (png_uint_16)(add & 0xffff);
      add >>= 16;
 }

 while (add != 0 && add != (-1))
 {
      num[out_digits++] = (png_uint_16)(add & 0xffff);
      add >>= 16;
 }

 if (add == 0)
 {
 while (out_digits > 0 && num[out_digits-1] == 0)
 --out_digits;
 return out_digits;  
 }

 else  
 {
 while (out_digits > 1 && num[out_digits-1] == 0xffff)
 --out_digits;

 return -out_digits;
 }
}
