png_read_chunk_header(png_structrp png_ptr)
{
   png_byte buf[8];
   png_uint_32 length;

#ifdef PNG_IO_STATE_SUPPORTED
   png_ptr->io_state = PNG_IO_READING | PNG_IO_CHUNK_HDR;
#endif

    
   png_read_data(png_ptr, buf, 8);
   length = png_get_uint_31(png_ptr, buf);

    
   png_ptr->chunk_name = PNG_CHUNK_FROM_STRING(buf+4);

   png_debug2(0, "Reading %lx chunk, length = %lu",
       (unsigned long)png_ptr->chunk_name, (unsigned long)length);

    
   png_reset_crc(png_ptr);
   png_calculate_crc(png_ptr, buf + 4, 4);

    
   png_check_chunk_name(png_ptr, png_ptr->chunk_name);

    
   png_check_chunk_length(png_ptr, length);

#ifdef PNG_IO_STATE_SUPPORTED
   png_ptr->io_state = PNG_IO_READING | PNG_IO_CHUNK_DATA;
#endif

   return length;
}
