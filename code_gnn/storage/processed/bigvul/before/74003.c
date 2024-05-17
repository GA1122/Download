static MagickBooleanType SkipRGBMipmaps(Image *image,DDSInfo *dds_info,
  int pixel_size,ExceptionInfo *exception)
{
  MagickOffsetType
    offset;

  register ssize_t
    i;

  size_t
    h,
    w;

   
  if (EOFBlob(image) != MagickFalse)
    {
      ThrowFileException(exception,CorruptImageError,"UnexpectedEndOfFile",
        image->filename);
      return(MagickFalse);
    }
  if (dds_info->ddscaps1 & DDSCAPS_MIPMAP
      && (dds_info->ddscaps1 & DDSCAPS_TEXTURE
          || dds_info->ddscaps2 & DDSCAPS2_CUBEMAP))
    {
      w = DIV2(dds_info->width);
      h = DIV2(dds_info->height);

       
      for (i=1; (i < (ssize_t) dds_info->mipmapcount) && w && h; i++)
      {
        offset = (MagickOffsetType) w * h * pixel_size;
        (void) SeekBlob(image, offset, SEEK_CUR);

        w = DIV2(w);
        h = DIV2(h);
      }
    }
  return(MagickTrue);
}