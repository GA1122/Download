png_get_int_32)(png_const_bytep buf)
{
   png_uint_32 uval = png_get_uint_32(buf);
   if ((uval & 0x80000000) == 0)  
      return (png_int_32)uval;

   uval = (uval ^ 0xffffffff) + 1;   
   if ((uval & 0x80000000) == 0)  
      return -(png_int_32)uval;
    
   return 0;
}
