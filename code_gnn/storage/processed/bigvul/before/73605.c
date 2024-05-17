static inline MagickBooleanType AcquireCacheNexusPixels(
  const CacheInfo *restrict cache_info,NexusInfo *nexus_info,
  ExceptionInfo *exception)
{
  if (nexus_info->length != (MagickSizeType) ((size_t) nexus_info->length))
    return(MagickFalse);
  nexus_info->mapped=MagickFalse;
  nexus_info->cache=(PixelPacket *) MagickAssumeAligned(AcquireAlignedMemory(1,
    (size_t) nexus_info->length));
  if (nexus_info->cache == (PixelPacket *) NULL)
    {
      nexus_info->mapped=MagickTrue;
      nexus_info->cache=(PixelPacket *) MapBlob(-1,IOMode,0,(size_t)
        nexus_info->length);
    }
  if (nexus_info->cache == (PixelPacket *) NULL)
    {
      (void) ThrowMagickException(exception,GetMagickModule(),
        ResourceLimitError,"MemoryAllocationFailed","`%s'",
        cache_info->filename);
      return(MagickFalse);
    }
  return(MagickTrue);
}
