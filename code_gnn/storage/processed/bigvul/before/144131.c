png_write_chunk(png_structp png_ptr, png_bytep chunk_name,
   png_bytep data, png_size_t length)
{
   if (png_ptr == NULL)
      return;
   png_write_chunk_start(png_ptr, chunk_name, (png_uint_32)length);
   png_write_chunk_data(png_ptr, data, (png_size_t)length);
   png_write_chunk_end(png_ptr);
}
