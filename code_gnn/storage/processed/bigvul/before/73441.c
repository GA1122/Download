MagickExport MagickBooleanType CacheComponentGenesis(void)
{
  if (cache_semaphore == (SemaphoreInfo *) NULL)
    cache_semaphore=AllocateSemaphoreInfo();
  return(MagickTrue);
}
