png_set_tRNS_to_alpha(png_structp png_ptr)
{
   png_debug(1, "in png_set_tRNS_to_alpha");

   png_ptr->transformations |= (PNG_EXPAND | PNG_EXPAND_tRNS);
   png_ptr->flags &= ~PNG_FLAG_ROW_INIT;
}
