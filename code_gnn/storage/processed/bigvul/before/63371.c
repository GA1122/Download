static boolean ReadComment(j_decompress_ptr jpeg_info)
{
  ErrorManager
    *error_manager;

  Image
    *image;

  register unsigned char
    *p;

  register ssize_t
    i;

  size_t
    length;

  StringInfo
    *comment;

   
  error_manager=(ErrorManager *) jpeg_info->client_data;
  image=error_manager->image;
  length=(size_t) ((size_t) GetCharacter(jpeg_info) << 8);
  length+=GetCharacter(jpeg_info);
  if (length <= 2)
    return(TRUE);
  length-=2;
  comment=BlobToStringInfo((const void *) NULL,length);
  if (comment == (StringInfo *) NULL)
    {
      (void) ThrowMagickException(&image->exception,GetMagickModule(),
        ResourceLimitError,"MemoryAllocationFailed","`%s'",image->filename);
      return(FALSE);
    }
   
  error_manager->profile=comment;
  p=GetStringInfoDatum(comment);
  for (i=0; i < (ssize_t) GetStringInfoLength(comment); i++)
    *p++=(unsigned char) GetCharacter(jpeg_info);
  *p='\0';
  error_manager->profile=NULL;
  p=GetStringInfoDatum(comment);
  (void) SetImageProperty(image,"comment",(const char *) p);
  comment=DestroyStringInfo(comment);
  return(TRUE);
}
