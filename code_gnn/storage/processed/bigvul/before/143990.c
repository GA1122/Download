png_get_x_offset_inches(png_structp png_ptr, png_infop info_ptr)
{
   return ((float)png_get_x_offset_microns(png_ptr, info_ptr)
     *.00003937);
}
