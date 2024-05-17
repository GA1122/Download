png_decompress_chunk(png_structp png_ptr, int comp_type,
    png_size_t chunklength,
    png_size_t prefix_size, png_size_t *newlength)
{
    
   if (prefix_size > chunklength)
   {
       
      png_warning(png_ptr, "invalid chunklength");
      prefix_size = 0;  
   }

   else if (comp_type == PNG_COMPRESSION_TYPE_BASE)
   {
      png_size_t expanded_size = png_inflate(png_ptr,
                (png_bytep)(png_ptr->chunkdata + prefix_size),
                chunklength - prefix_size,
                0 , 0 );

       
#ifdef PNG_SET_CHUNK_MALLOC_LIMIT_SUPPORTED
      if (png_ptr->user_chunk_malloc_max &&
          (prefix_size + expanded_size >= png_ptr->user_chunk_malloc_max - 1))
#else
#  ifdef PNG_USER_CHUNK_MALLOC_MAX
      if ((PNG_USER_CHUNK_MALLOC_MAX > 0) &&
          prefix_size + expanded_size >= PNG_USER_CHUNK_MALLOC_MAX - 1)
#  endif
#endif
         png_warning(png_ptr, "Exceeded size limit while expanding chunk");

       
#if defined(PNG_SET_CHUNK_MALLOC_LIMIT_SUPPORTED) || \
    defined(PNG_USER_CHUNK_MALLOC_MAX)
      else
#endif
      if (expanded_size > 0)
      {
          
         png_size_t new_size = 0;
         png_charp text = NULL;
          
         if (prefix_size + expanded_size > prefix_size &&
             prefix_size + expanded_size < 0xffffffffU)
         {
            text = png_malloc_warn(png_ptr, prefix_size + expanded_size + 1);
         }

         if (text != NULL)
         {
            png_memcpy(text, png_ptr->chunkdata, prefix_size);
            new_size = png_inflate(png_ptr,
                (png_bytep)(png_ptr->chunkdata + prefix_size),
                chunklength - prefix_size,
                (png_bytep)(text + prefix_size), expanded_size);
            text[prefix_size + expanded_size] = 0;  

            if (new_size == expanded_size)
            {
               png_free(png_ptr, png_ptr->chunkdata);
               png_ptr->chunkdata = text;
               *newlength = prefix_size + expanded_size;
               return;  
            }

            png_warning(png_ptr, "png_inflate logic error");
            png_free(png_ptr, text);
         }
         else
          png_warning(png_ptr, "Not enough memory to decompress chunk.");
      }
   }

   else  
   {
#if defined(PNG_STDIO_SUPPORTED) && !defined(_WIN32_WCE)
      char umsg[50];

      png_snprintf(umsg, sizeof umsg, "Unknown zTXt compression type %d",
          comp_type);
      png_warning(png_ptr, umsg);
#else
      png_warning(png_ptr, "Unknown zTXt compression type");
#endif

       
   }

    
   {
      png_charp text = png_malloc_warn(png_ptr, prefix_size + 1);
      if (text != NULL)
      {
         if (prefix_size > 0)
            png_memcpy(text, png_ptr->chunkdata, prefix_size);
         png_free(png_ptr, png_ptr->chunkdata);
         png_ptr->chunkdata = text;

          
         *(png_ptr->chunkdata + prefix_size) = 0x00;
      }
       
   }

   *newlength = prefix_size;
}
