uarb_set(uarb result, png_alloc_size_t val)
  
{
 int ndigits = 0;

 while (val > 0)
 {
      result[ndigits++] = (png_uint_16)(val & 0xffff);
      val >>= 16;
 }

 return ndigits;
}
