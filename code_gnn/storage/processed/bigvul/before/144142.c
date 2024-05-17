png_write_oFFs(png_structp png_ptr, png_int_32 x_offset, png_int_32 y_offset,
   int unit_type)
{
#ifdef PNG_USE_LOCAL_ARRAYS
   PNG_oFFs;
#endif
   png_byte buf[9];

   png_debug(1, "in png_write_oFFs");

   if (unit_type >= PNG_OFFSET_LAST)
      png_warning(png_ptr, "Unrecognized unit type for oFFs chunk");

   png_save_int_32(buf, x_offset);
   png_save_int_32(buf + 4, y_offset);
   buf[8] = (png_byte)unit_type;

   png_write_chunk(png_ptr, (png_bytep)png_oFFs, buf, (png_size_t)9);
}
