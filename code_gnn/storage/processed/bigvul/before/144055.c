png_set_gray_1_2_4_to_8(png_structp png_ptr)
{
   png_debug(1, "in png_set_gray_1_2_4_to_8");

   if (png_ptr == NULL)
      return;

   png_ptr->transformations |= (PNG_EXPAND | PNG_EXPAND_tRNS);
}
