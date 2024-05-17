static IndexPacket *GetAuthenticIndexesFromCache(const Image *image)
{
  CacheInfo
    *restrict cache_info;

  const int
    id = GetOpenMPThreadId();

  assert(image != (const Image *) NULL);
  assert(image->signature == MagickSignature);
  assert(image->cache != (Cache) NULL);
  cache_info=(CacheInfo *) image->cache;
  assert(cache_info->signature == MagickSignature);
  assert(id < (int) cache_info->number_threads);
  return(cache_info->nexus_info[id]->indexes);
}
