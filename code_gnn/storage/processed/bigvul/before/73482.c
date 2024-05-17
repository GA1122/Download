MagickExport const IndexPacket *GetVirtualIndexQueue(const Image *image)
{
  CacheInfo
    *magick_restrict cache_info;

  const int
    id = GetOpenMPThreadId();

  assert(image != (const Image *) NULL);
  assert(image->signature == MagickSignature);
  assert(image->cache != (Cache) NULL);
  cache_info=(CacheInfo *) image->cache;
  assert(cache_info->signature == MagickSignature);
  if (cache_info->methods.get_virtual_indexes_from_handler !=
       (GetVirtualIndexesFromHandler) NULL)
    return(cache_info->methods.get_virtual_indexes_from_handler(image));
  assert(id < (int) cache_info->number_threads);
  return(GetVirtualIndexesFromNexus(cache_info,cache_info->nexus_info[id]));
}
