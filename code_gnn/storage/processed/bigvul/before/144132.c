png_write_chunk_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
    
   if (png_ptr == NULL)
      return;
   if (data != NULL && length > 0)
   {
      png_write_data(png_ptr, data, length);
       
      png_calculate_crc(png_ptr, data, length);
   }
}
