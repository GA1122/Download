MagickExport MagickBooleanType PersistPixelCache(Image *image,
  const char *filename,const MagickBooleanType attach,MagickOffsetType *offset,
  ExceptionInfo *exception)
{
  CacheInfo
    *restrict cache_info,
    *restrict clone_info;

  Image
    clone_image;

  MagickBooleanType
    status;

  ssize_t
    page_size;

  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  assert(image->cache != (void *) NULL);
  assert(filename != (const char *) NULL);
  assert(offset != (MagickOffsetType *) NULL);
  page_size=GetMagickPageSize();
  cache_info=(CacheInfo *) image->cache;
  assert(cache_info->signature == MagickSignature);
  if (attach != MagickFalse)
    {
       
      if (image->debug != MagickFalse)
        (void) LogMagickEvent(CacheEvent,GetMagickModule(),
          "attach persistent cache");
      (void) CopyMagickString(cache_info->cache_filename,filename,
        MaxTextExtent);
      cache_info->type=DiskCache;
      cache_info->offset=(*offset);
      if (OpenPixelCache(image,ReadMode,exception) == MagickFalse)
        return(MagickFalse);
      *offset+=cache_info->length+page_size-(cache_info->length % page_size);
      return(MagickTrue);
    }
  if ((cache_info->mode != ReadMode) && (cache_info->type != MemoryCache) &&
      (cache_info->reference_count == 1))
    {
      LockSemaphoreInfo(cache_info->semaphore);
      if ((cache_info->mode != ReadMode) && (cache_info->type != MemoryCache) &&
          (cache_info->reference_count == 1))
        {
          int
            status;

           
          status=rename_utf8(cache_info->cache_filename,filename);
          if (status == 0)
            {
              (void) CopyMagickString(cache_info->cache_filename,filename,
                MaxTextExtent);
              *offset+=cache_info->length+page_size-(cache_info->length %
                page_size);
              UnlockSemaphoreInfo(cache_info->semaphore);
              cache_info=(CacheInfo *) ReferencePixelCache(cache_info);
              if (image->debug != MagickFalse)
                (void) LogMagickEvent(CacheEvent,GetMagickModule(),
                  "Usurp resident persistent cache");
              return(MagickTrue);
            }
        }
      UnlockSemaphoreInfo(cache_info->semaphore);
    }
   
  clone_image=(*image);
  clone_info=(CacheInfo *) clone_image.cache;
  image->cache=ClonePixelCache(cache_info);
  cache_info=(CacheInfo *) ReferencePixelCache(image->cache);
  (void) CopyMagickString(cache_info->cache_filename,filename,MaxTextExtent);
  cache_info->type=DiskCache;
  cache_info->offset=(*offset);
  cache_info=(CacheInfo *) image->cache;
  status=OpenPixelCache(image,IOMode,exception);
  if (status != MagickFalse)
    status=ClonePixelCacheRepository(cache_info,clone_info,&image->exception);
  *offset+=cache_info->length+page_size-(cache_info->length % page_size);
  clone_info=(CacheInfo *) DestroyPixelCache(clone_info);
  return(status);
}
