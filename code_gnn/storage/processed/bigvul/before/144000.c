png_process_IDAT_data(png_structp png_ptr, png_bytep buffer,
   png_size_t buffer_length)
{
    
   if (!(buffer_length > 0) || buffer == NULL)
      png_error(png_ptr, "No IDAT data (internal error)");

    
   png_ptr->zstream.next_in = buffer;
   png_ptr->zstream.avail_in = (uInt)buffer_length;

    
   while (png_ptr->zstream.avail_in > 0 &&
	  !(png_ptr->flags & PNG_FLAG_ZLIB_FINISHED))
   {
      int ret;

       
      if (!(png_ptr->zstream.avail_out > 0))
      {
         png_ptr->zstream.avail_out =
             (uInt) PNG_ROWBYTES(png_ptr->pixel_depth,
             png_ptr->iwidth) + 1;
         png_ptr->zstream.next_out = png_ptr->row_buf;
      }

       
      ret = inflate(&png_ptr->zstream, Z_SYNC_FLUSH);

       
      if (ret != Z_OK && ret != Z_STREAM_END)
      {
	  
	 png_ptr->flags |= PNG_FLAG_ZLIB_FINISHED;

          
         if (png_ptr->row_number >= png_ptr->num_rows ||
	     png_ptr->pass > 6)
	    png_warning(png_ptr, "Truncated compressed data in IDAT");
	 else
	    png_error(png_ptr, "Decompression error in IDAT");

	  
         return;
      }

       
      if (png_ptr->zstream.next_out != png_ptr->row_buf)
      {
	  
         if (png_ptr->row_number >= png_ptr->num_rows ||
	     png_ptr->pass > 6)
         {
	     
	    png_warning(png_ptr, "Extra compressed data in IDAT");
            png_ptr->flags |= PNG_FLAG_ZLIB_FINISHED;
	     
            return;
	 }

	  
	 if (png_ptr->zstream.avail_out == 0)
	    png_push_process_row(png_ptr);
      }

       
      if (ret == Z_STREAM_END)
	 png_ptr->flags |= PNG_FLAG_ZLIB_FINISHED;
   }

    
   if (png_ptr->zstream.avail_in > 0)
      png_warning(png_ptr, "Extra compression data");
}
