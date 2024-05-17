static int read_user_chunk_callback(png_struct *ping, png_unknown_chunkp chunk)
{
  Image
    *image;


   

  (void) LogMagickEvent(CoderEvent,GetMagickModule(),
     "    read_user_chunk: found %c%c%c%c chunk",
       chunk->name[0],chunk->name[1],chunk->name[2],chunk->name[3]);

  if (chunk->name[0]  == 101 &&
      (chunk->name[1] ==  88 || chunk->name[1] == 120 ) &&
      chunk->name[2] ==   73 &&
      chunk-> name[3] == 102)
    {
       

      PNGErrorInfo
        *error_info;

      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
        " recognized eXIf chunk");

      image=(Image *) png_get_user_chunk_ptr(ping);

      error_info=(PNGErrorInfo *) png_get_error_ptr(ping);

      return(PNGSetExifProfile(image,chunk->size,chunk->data,
        error_info->exception));
    }

   
  if (chunk->name[0] == 111 &&
      chunk->name[1] == 114 &&
      chunk->name[2] ==  78 &&
      chunk->name[3] ==  84)
    {
      
     if (chunk->size != 1)
       return(-1);  

     image=(Image *) png_get_user_chunk_ptr(ping);

     image->orientation=
       Magick_Orientation_from_Exif_Orientation((int) chunk->data[0]);

     return(1);
    }

   
  if (chunk->name[0] == 118 &&
      chunk->name[1] == 112 &&
      chunk->name[2] ==  65 &&
      chunk->name[3] == 103)
    {
       

      if (chunk->size != 9)
        return(-1);  

      if (chunk->data[8] != 0)
        return(0);   

      image=(Image *) png_get_user_chunk_ptr(ping);

      image->page.width=(size_t)mng_get_long(chunk->data);
      image->page.height=(size_t)mng_get_long(&chunk->data[4]);

      return(1);
    }

   
  if (chunk->name[0] ==  99 &&
      chunk->name[1] ==  97 &&
      chunk->name[2] ==  78 &&
      chunk->name[3] == 118)
    {
       

      if (chunk->size != 16)
        return(-1);  

      image=(Image *) png_get_user_chunk_ptr(ping);

      image->page.width=(size_t)mng_get_long(chunk->data);
      image->page.height=(size_t)mng_get_long(&chunk->data[4]);
      image->page.x=(size_t)mng_get_long(&chunk->data[8]);
      image->page.y=(size_t)mng_get_long(&chunk->data[12]);

      return(1);
    }

  return(0);  
}