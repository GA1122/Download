png_handle_iTXt(png_structp png_ptr, png_infop info_ptr, png_uint_32 length)
{
   png_textp text_ptr;
   png_charp key, lang, text, lang_key;
   int comp_flag;
   int comp_type = 0;
   int ret;
   png_size_t slength, prefix_len, data_len;

   png_debug(1, "in png_handle_iTXt");

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
         png_warning(png_ptr, "No space in chunk cache for iTXt");
         png_crc_finish(png_ptr, length);
         return;
      }
   }
#endif

   if (!(png_ptr->mode & PNG_HAVE_IHDR))
      png_error(png_ptr, "Missing IHDR before iTXt");

   if (png_ptr->mode & PNG_HAVE_IDAT)
      png_ptr->mode |= PNG_AFTER_IDAT;

#ifdef PNG_MAX_MALLOC_64K
    
   if (length > (png_uint_32)65535L)
   {
     png_warning(png_ptr, "iTXt chunk too large to fit in memory");
     png_crc_finish(png_ptr, length);
     return;
   }
#endif

   png_free(png_ptr, png_ptr->chunkdata);
   png_ptr->chunkdata = (png_charp)png_malloc_warn(png_ptr, length + 1);
   if (png_ptr->chunkdata == NULL)
   {
     png_warning(png_ptr, "No memory to process iTXt chunk.");
     return;
   }
   slength = (png_size_t)length;
   png_crc_read(png_ptr, (png_bytep)png_ptr->chunkdata, slength);
   if (png_crc_finish(png_ptr, 0))
   {
      png_free(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }

   png_ptr->chunkdata[slength] = 0x00;

   for (lang = png_ptr->chunkdata; *lang; lang++)
        ;
   lang++;         

    

   if (lang >= png_ptr->chunkdata + slength - 3)
   {
      png_warning(png_ptr, "Truncated iTXt chunk");
      png_free(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }
   else
   {
       comp_flag = *lang++;
       comp_type = *lang++;
   }

   for (lang_key = lang; *lang_key; lang_key++)
        ;
   lang_key++;         

   if (lang_key >= png_ptr->chunkdata + slength)
   {
      png_warning(png_ptr, "Truncated iTXt chunk");
      png_free(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }

   for (text = lang_key; *text; text++)
        ;
   text++;         
   if (text >= png_ptr->chunkdata + slength)
   {
      png_warning(png_ptr, "Malformed iTXt chunk");
      png_free(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }

   prefix_len = text - png_ptr->chunkdata;

   key=png_ptr->chunkdata;
   if (comp_flag)
       png_decompress_chunk(png_ptr, comp_type,
         (size_t)length, prefix_len, &data_len);
   else
       data_len = png_strlen(png_ptr->chunkdata + prefix_len);
   text_ptr = (png_textp)png_malloc_warn(png_ptr,
      (png_uint_32)png_sizeof(png_text));
   if (text_ptr == NULL)
   {
     png_warning(png_ptr, "Not enough memory to process iTXt chunk.");
     png_free(png_ptr, png_ptr->chunkdata);
     png_ptr->chunkdata = NULL;
     return;
   }
   text_ptr->compression = (int)comp_flag + 1;
   text_ptr->lang_key = png_ptr->chunkdata + (lang_key - key);
   text_ptr->lang = png_ptr->chunkdata + (lang - key);
   text_ptr->itxt_length = data_len;
   text_ptr->text_length = 0;
   text_ptr->key = png_ptr->chunkdata;
   text_ptr->text = png_ptr->chunkdata + prefix_len;

   ret = png_set_text_2(png_ptr, info_ptr, text_ptr, 1);

   png_free(png_ptr, text_ptr);
   png_free(png_ptr, png_ptr->chunkdata);
   png_ptr->chunkdata = NULL;
   if (ret)
     png_error(png_ptr, "Insufficient memory to store iTXt chunk.");
}