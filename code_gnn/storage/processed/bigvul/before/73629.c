MagickExport MagickSizeType GetPixelCacheNexusExtent(const Cache cache,
  NexusInfo *nexus_info)
{
  CacheInfo
    *restrict cache_info;

  MagickSizeType
    extent;

  assert(cache != NULL);
  cache_info=(CacheInfo *) cache;
  assert(cache_info->signature == MagickSignature);
  extent=(MagickSizeType) nexus_info->region.width*nexus_info->region.height;
  if (extent == 0)
    return((MagickSizeType) cache_info->columns*cache_info->rows);
  return(extent);
}
