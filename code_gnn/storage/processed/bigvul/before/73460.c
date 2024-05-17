MagickExport PixelPacket *GetAuthenticPixelCacheNexus(Image *image,
  const ssize_t x,const ssize_t y,const size_t columns,const size_t rows,
  NexusInfo *nexus_info,ExceptionInfo *exception)
{
  CacheInfo
    *magick_restrict cache_info;

  PixelPacket
    *magick_restrict pixels;

   
  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  pixels=QueueAuthenticPixelCacheNexus(image,x,y,columns,rows,MagickTrue,
    nexus_info,exception);
  if (pixels == (PixelPacket *) NULL)
    return((PixelPacket *) NULL);
  cache_info=(CacheInfo *) image->cache;
  assert(cache_info->signature == MagickSignature);
  if (nexus_info->authentic_pixel_cache != MagickFalse)
    return(pixels);
  if (ReadPixelCachePixels(cache_info,nexus_info,exception) == MagickFalse)
    return((PixelPacket *) NULL);
  if (cache_info->active_index_channel != MagickFalse)
    if (ReadPixelCacheIndexes(cache_info,nexus_info,exception) == MagickFalse)
      return((PixelPacket *) NULL);
  return(pixels);
}
