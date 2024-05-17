png_do_dither(png_row_infop row_info, png_bytep row,
    png_bytep palette_lookup, png_bytep dither_lookup)
{
   png_bytep sp, dp;
   png_uint_32 i;
   png_uint_32 row_width=row_info->width;

   png_debug(1, "in png_do_dither");

#ifdef PNG_USELESS_TESTS_SUPPORTED
   if (row != NULL && row_info != NULL)
#endif
   {
      if (row_info->color_type == PNG_COLOR_TYPE_RGB &&
         palette_lookup && row_info->bit_depth == 8)
      {
         int r, g, b, p;
         sp = row;
         dp = row;
         for (i = 0; i < row_width; i++)
         {
            r = *sp++;
            g = *sp++;
            b = *sp++;

             
            p = (((r >> (8 - PNG_DITHER_RED_BITS)) &
               ((1 << PNG_DITHER_RED_BITS) - 1)) <<
               (PNG_DITHER_GREEN_BITS + PNG_DITHER_BLUE_BITS)) |
               (((g >> (8 - PNG_DITHER_GREEN_BITS)) &
               ((1 << PNG_DITHER_GREEN_BITS) - 1)) <<
               (PNG_DITHER_BLUE_BITS)) |
               ((b >> (8 - PNG_DITHER_BLUE_BITS)) &
               ((1 << PNG_DITHER_BLUE_BITS) - 1));

            *dp++ = palette_lookup[p];
         }
         row_info->color_type = PNG_COLOR_TYPE_PALETTE;
         row_info->channels = 1;
         row_info->pixel_depth = row_info->bit_depth;
         row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth, row_width);
      }
      else if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA &&
         palette_lookup != NULL && row_info->bit_depth == 8)
      {
         int r, g, b, p;
         sp = row;
         dp = row;
         for (i = 0; i < row_width; i++)
         {
            r = *sp++;
            g = *sp++;
            b = *sp++;
            sp++;

            p = (((r >> (8 - PNG_DITHER_RED_BITS)) &
               ((1 << PNG_DITHER_RED_BITS) - 1)) <<
               (PNG_DITHER_GREEN_BITS + PNG_DITHER_BLUE_BITS)) |
               (((g >> (8 - PNG_DITHER_GREEN_BITS)) &
               ((1 << PNG_DITHER_GREEN_BITS) - 1)) <<
               (PNG_DITHER_BLUE_BITS)) |
               ((b >> (8 - PNG_DITHER_BLUE_BITS)) &
               ((1 << PNG_DITHER_BLUE_BITS) - 1));

            *dp++ = palette_lookup[p];
         }
         row_info->color_type = PNG_COLOR_TYPE_PALETTE;
         row_info->channels = 1;
         row_info->pixel_depth = row_info->bit_depth;
         row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth, row_width);
      }
      else if (row_info->color_type == PNG_COLOR_TYPE_PALETTE &&
         dither_lookup && row_info->bit_depth == 8)
      {
         sp = row;
         for (i = 0; i < row_width; i++, sp++)
         {
            *sp = dither_lookup[*sp];
         }
      }
   }
}
