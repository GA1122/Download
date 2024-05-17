png_write_end(png_structp png_ptr, png_infop info_ptr)
{
   png_debug(1, "in png_write_end");

   if (png_ptr == NULL)
      return;
   if (!(png_ptr->mode & PNG_HAVE_IDAT))
      png_error(png_ptr, "No IDATs written into file");

    
   if (info_ptr != NULL)
   {
#ifdef PNG_WRITE_TEXT_SUPPORTED
      int i;  
#endif
#ifdef PNG_WRITE_tIME_SUPPORTED
       
      if ((info_ptr->valid & PNG_INFO_tIME) &&
         !(png_ptr->mode & PNG_WROTE_tIME))
         png_write_tIME(png_ptr, &(info_ptr->mod_time));
#endif
#ifdef PNG_WRITE_TEXT_SUPPORTED
       
      for (i = 0; i < info_ptr->num_text; i++)
      {
         png_debug2(2, "Writing trailer text chunk %d, type %d", i,
            info_ptr->text[i].compression);
          
         if (info_ptr->text[i].compression > 0)
         {
#ifdef PNG_WRITE_iTXt_SUPPORTED
             
            png_write_iTXt(png_ptr,
                        info_ptr->text[i].compression,
                        info_ptr->text[i].key,
                        info_ptr->text[i].lang,
                        info_ptr->text[i].lang_key,
                        info_ptr->text[i].text);
#else
            png_warning(png_ptr, "Unable to write international text");
#endif
             
            info_ptr->text[i].compression = PNG_TEXT_COMPRESSION_NONE_WR;
         }
         else if (info_ptr->text[i].compression >= PNG_TEXT_COMPRESSION_zTXt)
         {
#ifdef PNG_WRITE_zTXt_SUPPORTED
             
            png_write_zTXt(png_ptr, info_ptr->text[i].key,
               info_ptr->text[i].text, 0,
               info_ptr->text[i].compression);
#else
            png_warning(png_ptr, "Unable to write compressed text");
#endif
             
            info_ptr->text[i].compression = PNG_TEXT_COMPRESSION_zTXt_WR;
         }
         else if (info_ptr->text[i].compression == PNG_TEXT_COMPRESSION_NONE)
         {
#ifdef PNG_WRITE_tEXt_SUPPORTED
             
            png_write_tEXt(png_ptr, info_ptr->text[i].key,
               info_ptr->text[i].text, 0);
#else
            png_warning(png_ptr, "Unable to write uncompressed text");
#endif

             
            info_ptr->text[i].compression = PNG_TEXT_COMPRESSION_NONE_WR;
         }
      }
#endif
#ifdef PNG_WRITE_UNKNOWN_CHUNKS_SUPPORTED
   if (info_ptr->unknown_chunks_num)
   {
      png_unknown_chunk *up;

      png_debug(5, "writing extra chunks");

      for (up = info_ptr->unknown_chunks;
           up < info_ptr->unknown_chunks + info_ptr->unknown_chunks_num;
           up++)
      {
         int keep = png_handle_as_unknown(png_ptr, up->name);
         if (keep != PNG_HANDLE_CHUNK_NEVER &&
            up->location && (up->location & PNG_AFTER_IDAT) &&
            ((up->name[3] & 0x20) || keep == PNG_HANDLE_CHUNK_ALWAYS ||
            (png_ptr->flags & PNG_FLAG_KEEP_UNSAFE_CHUNKS)))
         {
            png_write_chunk(png_ptr, up->name, up->data, up->size);
         }
      }
   }
#endif
   }

   png_ptr->mode |= PNG_AFTER_IDAT;

    
   png_write_IEND(png_ptr);
    
#ifdef PNG_WRITE_FLUSH_SUPPORTED
#  ifdef PNG_WRITE_FLUSH_AFTER_IEND_SUPPORTED
   png_flush(png_ptr);
#  endif
#endif
}
