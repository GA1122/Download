png_read_finish_IDAT(png_structrp png_ptr)
{
    
   if ((png_ptr->flags & PNG_FLAG_ZSTREAM_ENDED) == 0)
   {
       
      png_read_IDAT_data(png_ptr, NULL, 0);
      png_ptr->zstream.next_out = NULL;  

       
      if ((png_ptr->flags & PNG_FLAG_ZSTREAM_ENDED) == 0)
      {
         png_ptr->mode |= PNG_AFTER_IDAT;
         png_ptr->flags |= PNG_FLAG_ZSTREAM_ENDED;
      }
   }

    
   if (png_ptr->zowner == png_IDAT)
   {
       
      png_ptr->zstream.next_in = NULL;
      png_ptr->zstream.avail_in = 0;

       
      png_ptr->zowner = 0;

       
      (void)png_crc_finish(png_ptr, png_ptr->idat_size);
   }
}
