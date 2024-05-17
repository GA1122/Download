png_inflate_claim(png_structrp png_ptr, png_uint_32 owner)
{
   if (png_ptr->zowner != 0)
   {
      char msg[64];

      PNG_STRING_FROM_CHUNK(msg, png_ptr->zowner);
       
      (void)png_safecat(msg, (sizeof msg), 4, " using zstream");
#if PNG_RELEASE_BUILD
      png_chunk_warning(png_ptr, msg);
      png_ptr->zowner = 0;
#else
      png_chunk_error(png_ptr, msg);
#endif
   }

    
   {
      int ret;  
#if ZLIB_VERNUM >= 0x1240
      int window_bits = 0;

# if defined(PNG_SET_OPTION_SUPPORTED) && defined(PNG_MAXIMUM_INFLATE_WINDOW)
      if (((png_ptr->options >> PNG_MAXIMUM_INFLATE_WINDOW) & 3) ==
          PNG_OPTION_ON)
      {
         window_bits = 15;
         png_ptr->zstream_start = 0;  
      }

      else
      {
         png_ptr->zstream_start = 1;
      }
# endif

#endif  

       
      png_ptr->zstream.next_in = NULL;
      png_ptr->zstream.avail_in = 0;
      png_ptr->zstream.next_out = NULL;
      png_ptr->zstream.avail_out = 0;

      if ((png_ptr->flags & PNG_FLAG_ZSTREAM_INITIALIZED) != 0)
      {
#if ZLIB_VERNUM >= 0x1240
         ret = inflateReset2(&png_ptr->zstream, window_bits);
#else
         ret = inflateReset(&png_ptr->zstream);
#endif
      }

      else
      {
#if ZLIB_VERNUM >= 0x1240
         ret = inflateInit2(&png_ptr->zstream, window_bits);
#else
         ret = inflateInit(&png_ptr->zstream);
#endif

         if (ret == Z_OK)
            png_ptr->flags |= PNG_FLAG_ZSTREAM_INITIALIZED;
      }

#if ZLIB_VERNUM >= 0x1290 && \
   defined(PNG_SET_OPTION_SUPPORTED) && defined(PNG_IGNORE_ADLER32)
      if (((png_ptr->options >> PNG_IGNORE_ADLER32) & 3) == PNG_OPTION_ON)
          
         ret = inflateValidate(&png_ptr->zstream, 0);
#endif

      if (ret == Z_OK)
         png_ptr->zowner = owner;

      else
         png_zstream_error(png_ptr, ret);

      return ret;
   }

#ifdef window_bits
# undef window_bits
#endif
}
