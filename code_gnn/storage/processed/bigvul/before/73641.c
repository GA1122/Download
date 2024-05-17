MagickExport const PixelPacket *GetVirtualPixelsNexus(const Cache cache,
  NexusInfo *nexus_info)
{
  CacheInfo
    *restrict cache_info;

  assert(cache != (Cache) NULL);
  cache_info=(CacheInfo *) cache;
  assert(cache_info->signature == MagickSignature);
  if (cache_info->storage_class == UndefinedClass)
    return((PixelPacket *) NULL);
  return((const PixelPacket *) nexus_info->pixels);
}
