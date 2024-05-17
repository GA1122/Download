MagickExport const void *AcquirePixelCachePixels(const Image *image,
  MagickSizeType *length,ExceptionInfo *exception)
{
  CacheInfo
    *restrict cache_info;

  assert(image != (const Image *) NULL);
  assert(image->signature == MagickSignature);
  assert(exception != (ExceptionInfo *) NULL);
  assert(exception->signature == MagickSignature);
  assert(image->cache != (Cache) NULL);
  cache_info=(CacheInfo *) image->cache;
  assert(cache_info->signature == MagickSignature);
  (void) exception;
  *length=0;
  if ((cache_info->type != MemoryCache) && (cache_info->type != MapCache))
    return((const void *) NULL);
  *length=cache_info->length;
  return((const void *) cache_info->pixels);
}
