static int read_user_chunk_callback(png_struct *ping, png_unknown_chunkp chunk)
{
  Image
    *image;


   

  LogMagickEvent(CoderEvent,GetMagickModule(),
     "    read_user_chunk: found %c%c%c%c chunk",
       chunk->name[0],chunk->name[1],chunk->name[2],chunk->name[3]);

  if (chunk->name[0]  == 101 &&
      (chunk->name[1] ==   88 || chunk->name[1] == 120 ) &&
      chunk->name[2] ==   73 &&
      chunk-> name[3] == 102)
    {
       

      PNGErrorInfo
        *error_info;

      StringInfo
        *profile;

      unsigned char
        *p;

      png_byte
        *s;

      int
        i;

      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
        " recognized eXIf|exIf chunk");

      image=(Image *) png_get_user_chunk_ptr(ping);

      error_info=(PNGErrorInfo *) png_get_error_ptr(ping);

      profile=BlobToStringInfo((const void *) NULL,chunk->size+6);
      if (profile == (StringInfo *) NULL)
        {
          (void) ThrowMagickException(error_info->exception,GetMagickModule(),
            ResourceLimitError,"MemoryAllocationFailed","`%s'",image->filename);
          return(-1);
        }
      p=GetStringInfoDatum(profile);

       
      *p++ ='E';
      *p++ ='x';
      *p++ ='i';
      *p++ ='f';
      *p++ ='\0';
      *p++ ='\0';

       
      s=chunk->data;
      for (i=0; i < (ssize_t) chunk->size; i++)
        *p++ = *s++;

      (void) SetImageProfile(image,"exif",profile);

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

     image->page.width=(size_t) ((chunk->data[0] << 24) |
        (chunk->data[1] << 16) | (chunk->data[2] << 8) | chunk->data[3]);

     image->page.height=(size_t) ((chunk->data[4] << 24) |
        (chunk->data[5] << 16) | (chunk->data[6] << 8) | chunk->data[7]);

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

     image->page.width=(size_t) ((chunk->data[0] << 24) |
        (chunk->data[1] << 16) | (chunk->data[2] << 8) | chunk->data[3]);

     image->page.height=(size_t) ((chunk->data[4] << 24) |
        (chunk->data[5] << 16) | (chunk->data[6] << 8) | chunk->data[7]);

     image->page.x=(size_t) ((chunk->data[8] << 24) |
        (chunk->data[9] << 16) | (chunk->data[10] << 8) | chunk->data[11]);

     image->page.y=(size_t) ((chunk->data[12] << 24) |
        (chunk->data[13] << 16) | (chunk->data[14] << 8) | chunk->data[15]);

      
         
         
         

     return(1);
    }

  return(0);  
}
