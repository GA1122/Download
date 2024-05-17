static MagickBooleanType SetPixelCacheExtent(Image *image,MagickSizeType length)
{
  CacheInfo
    *restrict cache_info;

  MagickOffsetType
    count,
    extent,
    offset;

  cache_info=(CacheInfo *) image->cache;
  if (image->debug != MagickFalse)
    {
      char
        format[MaxTextExtent],
        message[MaxTextExtent];

      (void) FormatMagickSize(length,MagickFalse,format);
      (void) FormatLocaleString(message,MaxTextExtent,
        "extend %s (%s[%d], disk, %s)",cache_info->filename,
        cache_info->cache_filename,cache_info->file,format);
      (void) LogMagickEvent(CacheEvent,GetMagickModule(),"%s",message);
    }
  if (length != (MagickSizeType) ((MagickOffsetType) length))
    return(MagickFalse);
  offset=(MagickOffsetType) lseek(cache_info->file,0,SEEK_END);
  if (offset < 0)
    return(MagickFalse);
  if ((MagickSizeType) offset >= length)
    return(MagickTrue);
  extent=(MagickOffsetType) length-1;
  count=WritePixelCacheRegion(cache_info,extent,1,(const unsigned char *) "");
#if defined(MAGICKCORE_HAVE_POSIX_FALLOCATE)
  if (cache_info->synchronize != MagickFalse)
    (void) posix_fallocate(cache_info->file,offset+1,extent-offset);
#endif
#if defined(SIGBUS)
  (void) signal(SIGBUS,CacheSignalHandler);
#endif
  return(count != (MagickOffsetType) 1 ? MagickFalse : MagickTrue);
}
