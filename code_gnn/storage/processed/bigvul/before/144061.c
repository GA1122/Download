png_set_strip_16(png_structp png_ptr)
{
   png_debug(1, "in png_set_strip_16");

   if (png_ptr == NULL)
      return;
   png_ptr->transformations |= PNG_16_TO_8;
}
