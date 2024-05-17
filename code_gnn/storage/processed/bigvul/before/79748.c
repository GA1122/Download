png_handle_unknown(png_structrp png_ptr, png_inforp info_ptr,
    png_uint_32 length, int keep)
{
   int handled = 0;  

   png_debug(1, "in png_handle_unknown");

#ifdef PNG_READ_UNKNOWN_CHUNKS_SUPPORTED
    
#  ifndef PNG_HANDLE_AS_UNKNOWN_SUPPORTED
#     ifdef PNG_SET_UNKNOWN_CHUNKS_SUPPORTED
   keep = png_chunk_unknown_handling(png_ptr, png_ptr->chunk_name);
#     endif
#  endif

    
#  ifdef PNG_READ_USER_CHUNKS_SUPPORTED
    
   if (png_ptr->read_user_chunk_fn != NULL)
   {
      if (png_cache_unknown_chunk(png_ptr, length) != 0)
      {
          
         int ret = (*(png_ptr->read_user_chunk_fn))(png_ptr,
             &png_ptr->unknown_chunk);

          
         if (ret < 0)
            png_chunk_error(png_ptr, "error in user chunk");

         else if (ret == 0)
         {
             
            if (keep < PNG_HANDLE_CHUNK_IF_SAFE)
            {
#              ifdef PNG_SET_UNKNOWN_CHUNKS_SUPPORTED
               if (png_ptr->unknown_default < PNG_HANDLE_CHUNK_IF_SAFE)
               {
                  png_chunk_warning(png_ptr, "Saving unknown chunk:");
                  png_app_warning(png_ptr,
                      "forcing save of an unhandled chunk;"
                      " please call png_set_keep_unknown_chunks");
                       
               }
#              endif
               keep = PNG_HANDLE_CHUNK_IF_SAFE;
            }
         }

         else  
         {
            handled = 1;
             
            keep = PNG_HANDLE_CHUNK_NEVER;
         }
      }

      else
         keep = PNG_HANDLE_CHUNK_NEVER;  
   }

   else
    
#  endif  

#  ifdef PNG_SAVE_UNKNOWN_CHUNKS_SUPPORTED
   {
       
      if (keep == PNG_HANDLE_CHUNK_AS_DEFAULT)
         keep = png_ptr->unknown_default;

      if (keep == PNG_HANDLE_CHUNK_ALWAYS ||
         (keep == PNG_HANDLE_CHUNK_IF_SAFE &&
          PNG_CHUNK_ANCILLARY(png_ptr->chunk_name)))
      {
         if (png_cache_unknown_chunk(png_ptr, length) == 0)
            keep = PNG_HANDLE_CHUNK_NEVER;
      }

      else
         png_crc_finish(png_ptr, length);
   }
#  else
#     ifndef PNG_READ_USER_CHUNKS_SUPPORTED
#        error no method to support READ_UNKNOWN_CHUNKS
#     endif

   {
       
      if (keep > PNG_HANDLE_CHUNK_NEVER)
         png_app_error(png_ptr, "no unknown chunk support available");

      png_crc_finish(png_ptr, length);
   }
#  endif

#  ifdef PNG_STORE_UNKNOWN_CHUNKS_SUPPORTED
    
   if (keep == PNG_HANDLE_CHUNK_ALWAYS ||
      (keep == PNG_HANDLE_CHUNK_IF_SAFE &&
       PNG_CHUNK_ANCILLARY(png_ptr->chunk_name)))
   {
#     ifdef PNG_USER_LIMITS_SUPPORTED
      switch (png_ptr->user_chunk_cache_max)
      {
         case 2:
            png_ptr->user_chunk_cache_max = 1;
            png_chunk_benign_error(png_ptr, "no space in chunk cache");
             
         case 1:
             
            break;

         default:  
            --(png_ptr->user_chunk_cache_max);
             
         case 0:  
#  endif  
             
            png_set_unknown_chunks(png_ptr, info_ptr,
                &png_ptr->unknown_chunk, 1);
            handled = 1;
#  ifdef PNG_USER_LIMITS_SUPPORTED
            break;
      }
#  endif
   }
#  else  
   PNG_UNUSED(info_ptr)
#  endif

    
   if (png_ptr->unknown_chunk.data != NULL)
      png_free(png_ptr, png_ptr->unknown_chunk.data);
   png_ptr->unknown_chunk.data = NULL;

#else  
    
   png_crc_finish(png_ptr, length);
   PNG_UNUSED(info_ptr)
   PNG_UNUSED(keep)
#endif  

    
   if (handled == 0 && PNG_CHUNK_CRITICAL(png_ptr->chunk_name))
      png_chunk_error(png_ptr, "unhandled critical chunk");
}
