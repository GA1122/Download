static inline void RelinquishCacheNexusPixels(NexusInfo *nexus_info)
{
  if (nexus_info->mapped == MagickFalse)
    (void) RelinquishAlignedMemory(nexus_info->cache);
  else
    (void) UnmapBlob(nexus_info->cache,(size_t) nexus_info->length);
  nexus_info->cache=(PixelPacket *) NULL;
  nexus_info->pixels=(PixelPacket *) NULL;
  nexus_info->indexes=(IndexPacket *) NULL;
  nexus_info->length=0;
  nexus_info->mapped=MagickFalse;
}
