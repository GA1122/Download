MagickExport PixelPacket *QueueAuthenticPixel(Image *image,const ssize_t x,
  const ssize_t y,const size_t columns,const size_t rows,
  const MagickBooleanType clone,NexusInfo *nexus_info,
  ExceptionInfo *exception)
{
  return(QueueAuthenticPixelCacheNexus(image,x,y,columns,rows,clone,nexus_info,
    exception));
}
