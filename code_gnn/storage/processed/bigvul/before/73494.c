static inline void PrefetchPixelCacheNexusPixels(const NexusInfo *nexus_info,
  const MapMode mode)
{
  magick_unreferenced(nexus_info);
  magick_unreferenced(mode);

  if (mode == ReadMode)
    {
      MagickCachePrefetch((unsigned char *) nexus_info->pixels,0,1);
      return;
    }
  MagickCachePrefetch((unsigned char *) nexus_info->pixels,1,1);
}