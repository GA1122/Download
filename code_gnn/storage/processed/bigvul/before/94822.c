static MagickBooleanType WritePixelCacheMetacontent(CacheInfo *cache_info,
  NexusInfo *magick_restrict nexus_info,ExceptionInfo *exception)
{
  MagickOffsetType
    count,
    offset;

  MagickSizeType
    extent,
    length;

  register const unsigned char
    *magick_restrict p;

  register ssize_t
    y;

  size_t
    rows;

  if (cache_info->metacontent_extent == 0)
    return(MagickFalse);
  if (nexus_info->authentic_pixel_cache != MagickFalse)
    return(MagickTrue);
  offset=(MagickOffsetType) nexus_info->region.y*cache_info->columns+
    nexus_info->region.x;
  length=(MagickSizeType) nexus_info->region.width*
    cache_info->metacontent_extent;
  extent=(MagickSizeType) length*nexus_info->region.height;
  rows=nexus_info->region.height;
  y=0;
  p=(unsigned char *) nexus_info->metacontent;
  switch (cache_info->type)
  {
    case MemoryCache:
    case MapCache:
    {
      register unsigned char
        *magick_restrict q;

       
      if ((cache_info->columns == nexus_info->region.width) &&
          (extent == (MagickSizeType) ((size_t) extent)))
        {
          length=extent;
          rows=1UL;
        }
      q=(unsigned char *) cache_info->metacontent+offset*
        cache_info->metacontent_extent;
      for (y=0; y < (ssize_t) rows; y++)
      {
        (void) memcpy(q,p,(size_t) length);
        p+=nexus_info->region.width*cache_info->metacontent_extent;
        q+=cache_info->columns*cache_info->metacontent_extent;
      }
      break;
    }
    case DiskCache:
    {
       
      LockSemaphoreInfo(cache_info->file_semaphore);
      if (OpenPixelCacheOnDisk(cache_info,IOMode) == MagickFalse)
        {
          ThrowFileException(exception,FileOpenError,"UnableToOpenFile",
            cache_info->cache_filename);
          UnlockSemaphoreInfo(cache_info->file_semaphore);
          return(MagickFalse);
        }
      if ((cache_info->columns == nexus_info->region.width) &&
          (extent <= MagickMaxBufferExtent))
        {
          length=extent;
          rows=1UL;
        }
      extent=(MagickSizeType) cache_info->columns*cache_info->rows;
      for (y=0; y < (ssize_t) rows; y++)
      {
        count=WritePixelCacheRegion(cache_info,cache_info->offset+extent*
          cache_info->number_channels*sizeof(Quantum)+offset*
          cache_info->metacontent_extent,length,(const unsigned char *) p);
        if (count != (MagickOffsetType) length)
          break;
        p+=cache_info->metacontent_extent*nexus_info->region.width;
        offset+=cache_info->columns;
      }
      if (IsFileDescriptorLimitExceeded() != MagickFalse)
        (void) ClosePixelCacheOnDisk(cache_info);
      UnlockSemaphoreInfo(cache_info->file_semaphore);
      break;
    }
    case DistributedCache:
    {
      RectangleInfo
        region;

       
      LockSemaphoreInfo(cache_info->file_semaphore);
      region=nexus_info->region;
      if ((cache_info->columns != nexus_info->region.width) ||
          (extent > MagickMaxBufferExtent))
        region.height=1UL;
      else
        {
          length=extent;
          rows=1UL;
        }
      for (y=0; y < (ssize_t) rows; y++)
      {
        count=WriteDistributePixelCacheMetacontent((DistributeCacheInfo *)
          cache_info->server_info,&region,length,(const unsigned char *) p);
        if (count != (MagickOffsetType) length)
          break;
        p+=cache_info->metacontent_extent*nexus_info->region.width;
        region.y++;
      }
      UnlockSemaphoreInfo(cache_info->file_semaphore);
      break;
    }
    default:
      break;
  }
  if (y < (ssize_t) rows)
    {
      ThrowFileException(exception,CacheError,"UnableToWritePixelCache",
        cache_info->cache_filename);
      return(MagickFalse);
    }
  if ((cache_info->debug != MagickFalse) &&
      (CacheTick(nexus_info->region.y,cache_info->rows) != MagickFalse))
    (void) LogMagickEvent(CacheEvent,GetMagickModule(),
      "%s[%.20gx%.20g%+.20g%+.20g]",cache_info->filename,(double)
      nexus_info->region.width,(double) nexus_info->region.height,(double)
      nexus_info->region.x,(double) nexus_info->region.y);
  return(MagickTrue);
}