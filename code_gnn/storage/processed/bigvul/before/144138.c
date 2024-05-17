png_write_gAMA_fixed(png_structp png_ptr, png_fixed_point file_gamma)
{
#ifdef PNG_USE_LOCAL_ARRAYS
   PNG_gAMA;
#endif
   png_byte buf[4];

   png_debug(1, "in png_write_gAMA");

    
   png_save_uint_32(buf, (png_uint_32)file_gamma);
   png_write_chunk(png_ptr, (png_bytep)png_gAMA, buf, (png_size_t)4);
}
