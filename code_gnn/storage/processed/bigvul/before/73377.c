MagickExport size_t GetImageDepth(const Image *image,ExceptionInfo *exception)
{
  return(GetImageChannelDepth(image,CompositeChannels,exception));
}
