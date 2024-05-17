static boolean ReadComment(j_decompress_ptr jpeg_info)
{
  ErrorManager
    *error_manager;

  ExceptionInfo
    *exception;

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
  exception=error_manager->exception;
  image=error_manager->image;
  length=(size_t) ((size_t) GetCharacter(jpeg_info) << 8);
  length+=GetCharacter(jpeg_info);
  if (length <= 2)
    return(TRUE);
  length-=2;
  comment=BlobToStringInfo((const void *) NULL,length);
  if (comment == (StringInfo *) NULL)
    {
      (void) ThrowMagickException(exception,GetMagickModule(),
        ResourceLimitError,"MemoryAllocationFailed","`%s'",image->filename);
      return(FALSE);
    }
   
  error_manager->profile=comment;
  p=GetStringInfoDatum(comment);
  for (i=0; i < (ssize_t) length; i++)
  {
    int
      c;

    c=GetCharacter(jpeg_info);
    if (c == EOF)
      break;
    *p++=(unsigned char) c;
  }
  *p='\0';
  error_manager->profile=NULL;
  if (i != (ssize_t) length)
    {
      comment=DestroyStringInfo(comment);
      (void) ThrowMagickException(exception,GetMagickModule(),
        CorruptImageError,"InsufficientImageDataInFile","`%s'",
        image->filename);
      return(FALSE);
    }
  p=GetStringInfoDatum(comment);
  (void) SetImageProperty(image,"comment",(const char *) p,exception);
  comment=DestroyStringInfo(comment);
  return(TRUE);
}