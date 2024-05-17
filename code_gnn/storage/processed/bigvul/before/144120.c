png_write_rows(png_structp png_ptr, png_bytepp row,
   png_uint_32 num_rows)
{
   png_uint_32 i;  
   png_bytepp rp;  

   png_debug(1, "in png_write_rows");

   if (png_ptr == NULL)
      return;

    
   for (i = 0, rp = row; i < num_rows; i++, rp++)
   {
      png_write_row(png_ptr, *rp);
   }
}
