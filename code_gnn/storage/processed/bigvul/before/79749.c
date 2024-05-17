png_handle_zTXt(png_structrp png_ptr, png_inforp info_ptr, png_uint_32 length)
{
   png_const_charp errmsg = NULL;
   png_bytep       buffer;
   png_uint_32     keyword_length;

   png_debug(1, "in png_handle_zTXt");

#ifdef PNG_USER_LIMITS_SUPPORTED
   if (png_ptr->user_chunk_cache_max != 0)
   {
      if (png_ptr->user_chunk_cache_max == 1)
      {
         png_crc_finish(png_ptr, length);
         return;
      }

      if (--png_ptr->user_chunk_cache_max == 1)
      {
         png_crc_finish(png_ptr, length);
         png_chunk_benign_error(png_ptr, "no space in chunk cache");
         return;
      }
   }
#endif

   if ((png_ptr->mode & PNG_HAVE_IHDR) == 0)
      png_chunk_error(png_ptr, "missing IHDR");

   if ((png_ptr->mode & PNG_HAVE_IDAT) != 0)
      png_ptr->mode |= PNG_AFTER_IDAT;

    
   buffer = png_read_buffer(png_ptr, length, 2 );

   if (buffer == NULL)
   {
      png_crc_finish(png_ptr, length);
      png_chunk_benign_error(png_ptr, "out of memory");
      return;
   }

   png_crc_read(png_ptr, buffer, length);

   if (png_crc_finish(png_ptr, 0) != 0)
      return;

    
   for (keyword_length = 0;
      keyword_length < length && buffer[keyword_length] != 0;
      ++keyword_length)
        ;

   if (keyword_length > 79 || keyword_length < 1)
      errmsg = "bad keyword";

    
   else if (keyword_length + 3 > length)
      errmsg = "truncated";

   else if (buffer[keyword_length+1] != PNG_COMPRESSION_TYPE_BASE)
      errmsg = "unknown compression type";

   else
   {
      png_alloc_size_t uncompressed_length = PNG_SIZE_MAX;

       
      if (png_decompress_chunk(png_ptr, length, keyword_length+2,
          &uncompressed_length, 1 ) == Z_STREAM_END)
      {
         png_text text;

         if (png_ptr->read_buffer == NULL)
           errmsg="Read failure in png_handle_zTXt";
         else
         {
             
            buffer = png_ptr->read_buffer;
            buffer[uncompressed_length+(keyword_length+2)] = 0;

            text.compression = PNG_TEXT_COMPRESSION_zTXt;
            text.key = (png_charp)buffer;
            text.text = (png_charp)(buffer + keyword_length+2);
            text.text_length = uncompressed_length;
            text.itxt_length = 0;
            text.lang = NULL;
            text.lang_key = NULL;

            if (png_set_text_2(png_ptr, info_ptr, &text, 1) != 0)
               errmsg = "insufficient memory";
         }
      }

      else
         errmsg = png_ptr->zstream.msg;
   }

   if (errmsg != NULL)
      png_chunk_benign_error(png_ptr, errmsg);
}