static inline MagickOffsetType WritePixelCacheRegion(
  const CacheInfo *restrict cache_info,const MagickOffsetType offset,
  const MagickSizeType length,const unsigned char *restrict buffer)
{
  register MagickOffsetType
    i;

  ssize_t
    count;

#if !defined(MAGICKCORE_HAVE_PWRITE)
  if (lseek(cache_info->file,offset,SEEK_SET) < 0)
    return((MagickOffsetType) -1);
#endif
  count=0;
  for (i=0; i < (MagickOffsetType) length; i+=count)
  {
#if !defined(MAGICKCORE_HAVE_PWRITE)
    count=write(cache_info->file,buffer+i,(size_t) MagickMin(length-i,
      (MagickSizeType) SSIZE_MAX));
#else
    count=pwrite(cache_info->file,buffer+i,(size_t) MagickMin(length-i,
      (MagickSizeType) SSIZE_MAX),(off_t) (offset+i));
#endif
    if (count <= 0)
      {
        count=0;
        if (errno != EINTR)
          break;
      }
  }
  return(i);
}