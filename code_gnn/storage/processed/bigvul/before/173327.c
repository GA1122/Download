uarb_add32(uarb num, int in_digits, png_uint_32 add)
  
{
 if (in_digits > 0)
 {
      in_digits = uarb_inc(num, in_digits, add & 0xffff);
 return uarb_inc(num+1, in_digits-1, add >> 16)+1;
 }

 return uarb_set(num, add);
}
