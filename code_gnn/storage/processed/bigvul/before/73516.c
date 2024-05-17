static inline MagickBooleanType ValidatePixelCacheMorphology(
  const Image *magick_restrict image)
{
  CacheInfo
    *magick_restrict cache_info;

   
  cache_info=(CacheInfo *) image->cache;
  if ((image->storage_class != cache_info->storage_class) ||
      (image->colorspace != cache_info->colorspace) ||
      (image->channels != cache_info->channels) ||
      (image->columns != cache_info->columns) ||
      (image->rows != cache_info->rows) ||
      (cache_info->nexus_info == (NexusInfo **) NULL))
    return(MagickFalse);
  return(MagickTrue);
}
