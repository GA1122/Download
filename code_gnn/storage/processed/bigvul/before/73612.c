MagickExport Cache DestroyPixelCache(Cache cache)
{
  CacheInfo
    *restrict cache_info;

  assert(cache != (Cache) NULL);
  cache_info=(CacheInfo *) cache;
  assert(cache_info->signature == MagickSignature);
  if (cache_info->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",
      cache_info->filename);
  LockSemaphoreInfo(cache_info->semaphore);
  cache_info->reference_count--;
  if (cache_info->reference_count != 0)
    {
      UnlockSemaphoreInfo(cache_info->semaphore);
      return((Cache) NULL);
    }
  UnlockSemaphoreInfo(cache_info->semaphore);
  if (cache_info->debug != MagickFalse)
    {
      char
        message[MaxTextExtent];

      (void) FormatLocaleString(message,MaxTextExtent,"destroy %s",
        cache_info->filename);
      (void) LogMagickEvent(CacheEvent,GetMagickModule(),"%s",message);
    }
  RelinquishPixelCachePixels(cache_info);
  if (cache_info->server_info != (DistributeCacheInfo *) NULL)
    cache_info->server_info=DestroyDistributeCacheInfo((DistributeCacheInfo *)
      cache_info->server_info);
  if (cache_info->nexus_info != (NexusInfo **) NULL)
    cache_info->nexus_info=DestroyPixelCacheNexus(cache_info->nexus_info,
      cache_info->number_threads);
  if (cache_info->random_info != (RandomInfo *) NULL)
    cache_info->random_info=DestroyRandomInfo(cache_info->random_info);
  if (cache_info->file_semaphore != (SemaphoreInfo *) NULL)
    DestroySemaphoreInfo(&cache_info->file_semaphore);
  if (cache_info->semaphore != (SemaphoreInfo *) NULL)
    DestroySemaphoreInfo(&cache_info->semaphore);
  cache_info->signature=(~MagickSignature);
  cache_info=(CacheInfo *) RelinquishMagickMemory(cache_info);
  cache=(Cache) NULL;
  return(cache);
}