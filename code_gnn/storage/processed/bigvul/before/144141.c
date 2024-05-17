png_write_iTXt(png_structp png_ptr, int compression, png_charp key,
    png_charp lang, png_charp lang_key, png_charp text)
{
#ifdef PNG_USE_LOCAL_ARRAYS
   PNG_iTXt;
#endif
   png_size_t lang_len, key_len, lang_key_len, text_len;
   png_charp new_lang;
   png_charp new_key = NULL;
   png_byte cbuf[2];
   compression_state comp;

   png_debug(1, "in png_write_iTXt");

   comp.num_output_ptr = 0;
   comp.max_output_ptr = 0;
   comp.output_ptr = NULL;
   comp.input = NULL;

   if ((key_len = png_check_keyword(png_ptr, key, &new_key))==0)
      return;

   if ((lang_len = png_check_keyword(png_ptr, lang, &new_lang))==0)
   {
      png_warning(png_ptr, "Empty language field in iTXt chunk");
      new_lang = NULL;
      lang_len = 0;
   }

   if (lang_key == NULL)
      lang_key_len = 0;
   else
      lang_key_len = png_strlen(lang_key);

   if (text == NULL)
      text_len = 0;
   else
      text_len = png_strlen(text);

    
   text_len = png_text_compress(png_ptr, text, text_len, compression-2,
      &comp);


    

   png_write_chunk_start(png_ptr, (png_bytep)png_iTXt,
          (png_uint_32)(
        5  
        + key_len
        + lang_len
        + lang_key_len
        + text_len));

    
   png_write_chunk_data(png_ptr, (png_bytep)new_key,
     (png_size_t)(key_len + 1));

    
   if (compression == PNG_ITXT_COMPRESSION_NONE || \
       compression == PNG_TEXT_COMPRESSION_NONE)
       cbuf[0] = 0;
   else  
       cbuf[0] = 1;
    
   cbuf[1] = 0;
   png_write_chunk_data(png_ptr, cbuf, (png_size_t)2);

   cbuf[0] = 0;
   png_write_chunk_data(png_ptr, (new_lang ? (png_bytep)new_lang : cbuf),
     (png_size_t)(lang_len + 1));
   png_write_chunk_data(png_ptr, (lang_key ? (png_bytep)lang_key : cbuf),
     (png_size_t)(lang_key_len + 1));
   png_write_compressed_data_out(png_ptr, &comp);

   png_write_chunk_end(png_ptr);
   png_free(png_ptr, new_key);
   png_free(png_ptr, new_lang);
}
