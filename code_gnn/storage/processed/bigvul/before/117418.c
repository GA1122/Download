png_read_chunk_header(png_structp png_ptr)
{
   png_byte buf[8];
   png_uint_32 length;

    
   png_read_data(png_ptr, buf, 8);
   length = png_get_uint_31(png_ptr, buf);

    
   png_memcpy(png_ptr->chunk_name, buf + 4, 4);

   png_debug2(0, "Reading %s chunk, length = %lu",
      png_ptr->chunk_name, length);

    
   png_reset_crc(png_ptr);
   png_calculate_crc(png_ptr, png_ptr->chunk_name, 4);

    
   png_check_chunk_name(png_ptr, png_ptr->chunk_name);

   return length;
}
