MagickExport Cache AcquirePixelCache(const size_t number_threads)
{
  CacheInfo
    *restrict cache_info;

  char
    *synchronize;

  cache_info=(CacheInfo *) AcquireQuantumMemory(1,sizeof(*cache_info));
  if (cache_info == (CacheInfo *) NULL)
    ThrowFatalException(ResourceLimitFatalError,"MemoryAllocationFailed");
  (void) ResetMagickMemory(cache_info,0,sizeof(*cache_info));
  cache_info->type=UndefinedCache;
  cache_info->mode=IOMode;
  cache_info->colorspace=sRGBColorspace;
  cache_info->channels=4;
  cache_info->file=(-1);
  cache_info->id=GetMagickThreadId();
  cache_info->number_threads=number_threads;
  if (GetOpenMPMaximumThreads() > cache_info->number_threads)
    cache_info->number_threads=GetOpenMPMaximumThreads();
  if (GetMagickResourceLimit(ThreadResource) > cache_info->number_threads)
    cache_info->number_threads=(size_t) GetMagickResourceLimit(ThreadResource);
  if (cache_info->number_threads == 0)
    cache_info->number_threads=1;
  cache_info->nexus_info=AcquirePixelCacheNexus(cache_info->number_threads);
  if (cache_info->nexus_info == (NexusInfo **) NULL)
    ThrowFatalException(ResourceLimitFatalError,"MemoryAllocationFailed");
  synchronize=GetEnvironmentValue("MAGICK_SYNCHRONIZE");
  if (synchronize != (const char *) NULL)
    {
      cache_info->synchronize=IsStringTrue(synchronize);
      synchronize=DestroyString(synchronize);
    }
  cache_info->semaphore=AllocateSemaphoreInfo();
  cache_info->reference_count=1;
  cache_info->file_semaphore=AllocateSemaphoreInfo();
  cache_info->debug=IsEventLogging();
  cache_info->signature=MagickSignature;
  return((Cache ) cache_info);
}