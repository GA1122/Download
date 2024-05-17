static MagickBooleanType WriteMVGImage(const ImageInfo *image_info,Image *image)
{
  const char
    *value;

  MagickBooleanType
    status;

   
  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickSignature);
  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  value=GetImageArtifact(image,"MVG");
  if (value == (const char *) NULL)
    ThrowWriterException(OptionError,"NoImageVectorGraphics");
  status=OpenBlob(image_info,image,WriteBlobMode,&image->exception);
  if (status == MagickFalse)
    return(status);
  (void) WriteBlob(image,strlen(value),(const unsigned char *) value);
  (void) CloseBlob(image);
  return(MagickTrue);
}
