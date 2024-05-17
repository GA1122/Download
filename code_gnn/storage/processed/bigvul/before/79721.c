png_decompress_chunk(png_structrp png_ptr,
    png_uint_32 chunklength, png_uint_32 prefix_size,
    png_alloc_size_t *newlength  ,
    int terminate  )
{
    
   png_alloc_size_t limit = PNG_SIZE_MAX;

# ifdef PNG_SET_USER_LIMITS_SUPPORTED
   if (png_ptr->user_chunk_malloc_max > 0 &&
       png_ptr->user_chunk_malloc_max < limit)
      limit = png_ptr->user_chunk_malloc_max;
# elif PNG_USER_CHUNK_MALLOC_MAX > 0
   if (PNG_USER_CHUNK_MALLOC_MAX < limit)
      limit = PNG_USER_CHUNK_MALLOC_MAX;
# endif

   if (limit >= prefix_size + (terminate != 0))
   {
      int ret;

      limit -= prefix_size + (terminate != 0);

      if (limit < *newlength)
         *newlength = limit;

       
      ret = png_inflate_claim(png_ptr, png_ptr->chunk_name);

      if (ret == Z_OK)
      {
         png_uint_32 lzsize = chunklength - prefix_size;

         ret = png_inflate(png_ptr, png_ptr->chunk_name, 1 ,
               png_ptr->read_buffer + prefix_size, &lzsize,
               NULL, newlength);

         if (ret == Z_STREAM_END)
         {
             
            if (inflateReset(&png_ptr->zstream) == Z_OK)
            {
                
               png_alloc_size_t new_size = *newlength;
               png_alloc_size_t buffer_size = prefix_size + new_size +
                   (terminate != 0);
               png_bytep text = png_voidcast(png_bytep, png_malloc_base(png_ptr,
                   buffer_size));

               if (text != NULL)
               {
                  memset(text, 0, buffer_size);

                  ret = png_inflate(png_ptr, png_ptr->chunk_name, 1 ,
                      png_ptr->read_buffer + prefix_size, &lzsize,
                      text + prefix_size, newlength);

                  if (ret == Z_STREAM_END)
                  {
                     if (new_size == *newlength)
                     {
                        if (terminate != 0)
                           text[prefix_size + *newlength] = 0;

                        if (prefix_size > 0)
                           memcpy(text, png_ptr->read_buffer, prefix_size);

                        {
                           png_bytep old_ptr = png_ptr->read_buffer;

                           png_ptr->read_buffer = text;
                           png_ptr->read_buffer_size = buffer_size;
                           text = old_ptr;  
                        }
                     }

                     else
                     {
                         
                        ret = PNG_UNEXPECTED_ZLIB_RETURN;
                     }
                  }

                  else if (ret == Z_OK)
                     ret = PNG_UNEXPECTED_ZLIB_RETURN;  

                   
                  png_free(png_ptr, text);

                   
                  if (ret == Z_STREAM_END &&
                      chunklength - prefix_size != lzsize)
                     png_chunk_benign_error(png_ptr, "extra compressed data");
               }

               else
               {
                   
                  ret = Z_MEM_ERROR;
                  png_zstream_error(png_ptr, Z_MEM_ERROR);
               }
            }

            else
            {
                
               png_zstream_error(png_ptr, ret);
               ret = PNG_UNEXPECTED_ZLIB_RETURN;
            }
         }

         else if (ret == Z_OK)
            ret = PNG_UNEXPECTED_ZLIB_RETURN;

          
         png_ptr->zowner = 0;
      }

      else   if (ret == Z_STREAM_END)  
         ret = PNG_UNEXPECTED_ZLIB_RETURN;

      return ret;
   }

   else
   {
       
      png_zstream_error(png_ptr, Z_MEM_ERROR);
      return Z_MEM_ERROR;
   }
}
