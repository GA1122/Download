png_write_tEXt(png_structp png_ptr, png_charp key, png_charp text,
   png_size_t text_len)
{
#ifdef PNG_USE_LOCAL_ARRAYS
   PNG_tEXt;
#endif
   png_size_t key_len;
   png_charp new_key;

   png_debug(1, "in png_write_tEXt");

   if ((key_len = png_check_keyword(png_ptr, key, &new_key))==0)
      return;

   if (text == NULL || *text == '\0')
      text_len = 0;
   else
      text_len = png_strlen(text);

    
   png_write_chunk_start(png_ptr, (png_bytep)png_tEXt,
      (png_uint_32)(key_len + text_len + 1));
    
   png_write_chunk_data(png_ptr, (png_bytep)new_key,
     (png_size_t)(key_len + 1));
   if (text_len)
      png_write_chunk_data(png_ptr, (png_bytep)text, (png_size_t)text_len);

   png_write_chunk_end(png_ptr);
   png_free(png_ptr, new_key);
}
