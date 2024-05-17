png_get_interlace_type(png_structp png_ptr, png_infop info_ptr)
{
   if (png_ptr != NULL && info_ptr != NULL)
      return info_ptr->interlace_type;

   return (0);
}
