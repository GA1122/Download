png_do_read_invert_alpha(png_row_infop row_info, png_bytep row)
{
   png_debug(1, "in png_do_read_invert_alpha");

#ifdef PNG_USELESS_TESTS_SUPPORTED
   if (row != NULL && row_info != NULL)
#endif
   {
      png_uint_32 row_width = row_info->width;
      if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
      {
          
         if (row_info->bit_depth == 8)
         {
            png_bytep sp = row + row_info->rowbytes;
            png_bytep dp = sp;
            png_uint_32 i;

            for (i = 0; i < row_width; i++)
            {
               *(--dp) = (png_byte)(255 - *(--sp));

 
               sp-=3;
               dp=sp;
            }
         }
          
         else
         {
            png_bytep sp = row + row_info->rowbytes;
            png_bytep dp = sp;
            png_uint_32 i;

            for (i = 0; i < row_width; i++)
            {
               *(--dp) = (png_byte)(255 - *(--sp));
               *(--dp) = (png_byte)(255 - *(--sp));

 
               sp-=6;
               dp=sp;
            }
         }
      }
      else if (row_info->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
      {
          
         if (row_info->bit_depth == 8)
         {
            png_bytep sp = row + row_info->rowbytes;
            png_bytep dp = sp;
            png_uint_32 i;

            for (i = 0; i < row_width; i++)
            {
               *(--dp) = (png_byte)(255 - *(--sp));
               *(--dp) = *(--sp);
            }
         }
          
         else
         {
            png_bytep sp  = row + row_info->rowbytes;
            png_bytep dp = sp;
            png_uint_32 i;

            for (i = 0; i < row_width; i++)
            {
               *(--dp) = (png_byte)(255 - *(--sp));
               *(--dp) = (png_byte)(255 - *(--sp));
 
               sp-=2;
               dp=sp;
            }
         }
      }
   }
}
