png_get_pixels_per_inch(png_structp png_ptr, png_infop info_ptr)
{
   return ((png_uint_32)((float)png_get_pixels_per_meter(png_ptr, info_ptr)
     *.0254 +.5));
}
