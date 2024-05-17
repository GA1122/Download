png_write_gAMA(png_structp png_ptr, double file_gamma)
{
#ifdef PNG_USE_LOCAL_ARRAYS
   PNG_gAMA;
#endif
   png_uint_32 igamma;
   png_byte buf[4];

   png_debug(1, "in png_write_gAMA");

    
   igamma = (png_uint_32)(file_gamma * 100000.0 + 0.5);
   png_save_uint_32(buf, igamma);
   png_write_chunk(png_ptr, (png_bytep)png_gAMA, buf, (png_size_t)4);
}
