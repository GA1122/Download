png_inflate(png_structrp png_ptr, png_uint_32 owner, int finish,
      png_const_bytep input, png_uint_32p input_size_ptr,
      png_bytep output, png_alloc_size_t *output_size_ptr)
{
   if (png_ptr->zowner == owner)  
   {
      int ret;
      png_alloc_size_t avail_out = *output_size_ptr;
      png_uint_32 avail_in = *input_size_ptr;

       
      png_ptr->zstream.next_in = PNGZ_INPUT_CAST(input);
       
      png_ptr->zstream.avail_in = 0;
      png_ptr->zstream.avail_out = 0;

       
      if (output != NULL)
         png_ptr->zstream.next_out = output;

      do
      {
         uInt avail;
         Byte local_buffer[PNG_INFLATE_BUF_SIZE];

          
          
         avail_in += png_ptr->zstream.avail_in;  

         avail = ZLIB_IO_MAX;

         if (avail_in < avail)
            avail = (uInt)avail_in;  

         avail_in -= avail;
         png_ptr->zstream.avail_in = avail;

          
         avail_out += png_ptr->zstream.avail_out;  

         avail = ZLIB_IO_MAX;  

         if (output == NULL)
         {
             
            png_ptr->zstream.next_out = local_buffer;
            if ((sizeof local_buffer) < avail)
               avail = (sizeof local_buffer);
         }

         if (avail_out < avail)
            avail = (uInt)avail_out;  

         png_ptr->zstream.avail_out = avail;
         avail_out -= avail;

          
          
         ret = PNG_INFLATE(png_ptr, avail_out > 0 ? Z_NO_FLUSH :
             (finish ? Z_FINISH : Z_SYNC_FLUSH));
      } while (ret == Z_OK);

       
      if (output == NULL)
         png_ptr->zstream.next_out = NULL;

       
      avail_in += png_ptr->zstream.avail_in;
      avail_out += png_ptr->zstream.avail_out;

       
      if (avail_out > 0)
         *output_size_ptr -= avail_out;

      if (avail_in > 0)
         *input_size_ptr -= avail_in;

       
      png_zstream_error(png_ptr, ret);
      return ret;
   }

   else
   {
       
      png_ptr->zstream.msg = PNGZ_MSG_CAST("zstream unclaimed");
      return Z_STREAM_ERROR;
   }
}
