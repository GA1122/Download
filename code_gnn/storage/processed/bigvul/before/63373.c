static boolean ReadIPTCProfile(j_decompress_ptr jpeg_info)
{
  char
    magick[MaxTextExtent];

  ErrorManager
    *error_manager;

  Image
    *image;

  MagickBooleanType
    status;

  register ssize_t
    i;

  register unsigned char
    *p;

  size_t
    length;

  StringInfo
    *iptc_profile,
    *profile;

   
  length=(size_t) ((size_t) GetCharacter(jpeg_info) << 8);
  length+=(size_t) GetCharacter(jpeg_info);
  length-=2;
  if (length <= 14)
    {
      while (length-- > 0)
        (void) GetCharacter(jpeg_info);
      return(TRUE);
    }
   
  for (i=0; i < 10; i++)
    magick[i]=(char) GetCharacter(jpeg_info);
  magick[10]='\0';
  length-=10;
  if (length <= 10)
    return(TRUE);
  if (LocaleCompare(magick,"Photoshop ") != 0)
    {
       
      for (i=0; i < (ssize_t) length; i++)
        (void) GetCharacter(jpeg_info);
      return(TRUE);
    }
   
  for (i=0; i < 4; i++)
    (void) GetCharacter(jpeg_info);
  if (length <= 11)
    return(TRUE);
  length-=4;
  error_manager=(ErrorManager *) jpeg_info->client_data;
  image=error_manager->image;
  profile=BlobToStringInfo((const void *) NULL,length);
  if (profile == (StringInfo *) NULL)
    {
      (void) ThrowMagickException(&image->exception,GetMagickModule(),
        ResourceLimitError,"MemoryAllocationFailed","`%s'",image->filename);
      return(FALSE);
    }
  error_manager->profile=profile;
  p=GetStringInfoDatum(profile);
  for (i=0;  i < (ssize_t) GetStringInfoLength(profile); i++)
    *p++=(unsigned char) GetCharacter(jpeg_info);
  error_manager->profile=NULL;
  iptc_profile=(StringInfo *) GetImageProfile(image,"8bim");
  if (iptc_profile != (StringInfo *) NULL)
    {
      ConcatenateStringInfo(iptc_profile,profile);
      profile=DestroyStringInfo(profile);
    }
  else
    {
      status=SetImageProfile(image,"8bim",profile);
      profile=DestroyStringInfo(profile);
      if (status == MagickFalse)
        {
          (void) ThrowMagickException(&image->exception,GetMagickModule(),
            ResourceLimitError,"MemoryAllocationFailed","`%s'",image->filename);
          return(FALSE);
        }
    }
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(CoderEvent,GetMagickModule(),
      "Profile: iptc, %.20g bytes",(double) length);
  return(TRUE);
}
