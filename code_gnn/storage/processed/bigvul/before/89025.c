MagickExport MagickBooleanType OrderedDitherImageChannel(Image *image,
 const ChannelType channel,ExceptionInfo *exception)
{
  MagickBooleanType
    status;

   
  status=OrderedPosterizeImageChannel(image,channel,"o8x8",exception);
  return(status);
}
