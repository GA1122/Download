png_set_gray_to_rgb(png_structp png_ptr)
{
   png_debug(1, "in png_set_gray_to_rgb");

   png_ptr->transformations |= PNG_GRAY_TO_RGB;
   png_ptr->flags &= ~PNG_FLAG_ROW_INIT;
}
