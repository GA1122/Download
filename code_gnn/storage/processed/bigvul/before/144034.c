png_do_chop(png_row_infop row_info, png_bytep row)
{
   png_debug(1, "in png_do_chop");

#ifdef PNG_USELESS_TESTS_SUPPORTED
   if (row != NULL && row_info != NULL && row_info->bit_depth == 16)
#else
   if (row_info->bit_depth == 16)
#endif
   {
      png_bytep sp = row;
      png_bytep dp = row;
      png_uint_32 i;
      png_uint_32 istop = row_info->width * row_info->channels;

      for (i = 0; i<istop; i++, sp += 2, dp++)
      {
#ifdef PNG_READ_16_TO_8_ACCURATE_SCALE_SUPPORTED
       

         *dp = *sp + ((((int)(*(sp + 1)) - *sp) > 128) ? 1 : 0);
#else
        
         *dp = *sp;
#endif
      }
      row_info->bit_depth = 8;
      row_info->pixel_depth = (png_byte)(8 * row_info->channels);
      row_info->rowbytes = row_info->width * row_info->channels;
   }
}
