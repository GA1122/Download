png_set_palette_to_rgb(png_structp png_ptr)
{
   png_debug(1, "in png_set_palette_to_rgb");

   if (png_ptr == NULL)
      return;

   png_ptr->transformations |= (PNG_EXPAND | PNG_EXPAND_tRNS);
   png_ptr->flags &= ~PNG_FLAG_ROW_INIT;
}
