png_write_chunk_start(png_structp png_ptr, png_bytep chunk_name,
   png_uint_32 length)
{
   png_byte buf[8];

   png_debug2(0, "Writing %s chunk, length = %lu", chunk_name,
      (unsigned long)length);

   if (png_ptr == NULL)
      return;


    
   png_save_uint_32(buf, length);
   png_memcpy(buf + 4, chunk_name, 4);
   png_write_data(png_ptr, buf, (png_size_t)8);
    
   png_memcpy(png_ptr->chunk_name, chunk_name, 4);
    
   png_reset_crc(png_ptr);
   png_calculate_crc(png_ptr, chunk_name, (png_size_t)4);
}
