png_set_strip_alpha(png_structp png_ptr)
{
   png_debug(1, "in png_set_strip_alpha");

   if (png_ptr == NULL)
      return;
   png_ptr->flags |= PNG_FLAG_STRIP_ALPHA;
}
