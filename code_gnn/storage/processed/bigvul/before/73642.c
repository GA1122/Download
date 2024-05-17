static inline MagickBooleanType IsAuthenticPixelCache(
  const CacheInfo *restrict cache_info,const NexusInfo *restrict nexus_info)
{
  MagickBooleanType
    status;

  MagickOffsetType
    offset;

   
  if (cache_info->type == PingCache)
    return(MagickTrue);
  offset=(MagickOffsetType) nexus_info->region.y*cache_info->columns+
    nexus_info->region.x;
  status=nexus_info->pixels == (cache_info->pixels+offset) ? MagickTrue :
    MagickFalse;
  return(status);
}
