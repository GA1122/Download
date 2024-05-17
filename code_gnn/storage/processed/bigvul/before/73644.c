static MagickBooleanType MaskPixelCacheNexus(Image *image,NexusInfo *nexus_info,
  ExceptionInfo *exception)
{
  CacheInfo
    *restrict cache_info;

  MagickPixelPacket
    alpha,
    beta;

  MagickSizeType
    number_pixels;

  NexusInfo
    **restrict clip_nexus,
    **restrict image_nexus;

  register const PixelPacket
    *restrict r;

  register IndexPacket
    *restrict nexus_indexes,
    *restrict indexes;

  register PixelPacket
    *restrict p,
    *restrict q;

  register ssize_t
    i;

   
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  if ((image->mask == (Image *) NULL) ||
      (image->storage_class == PseudoClass))
    return(MagickTrue);
  cache_info=(CacheInfo *) image->cache;
  if (cache_info == (Cache) NULL)
    return(MagickFalse);
  image_nexus=AcquirePixelCacheNexus(1);
  clip_nexus=AcquirePixelCacheNexus(1);
  if ((image_nexus == (NexusInfo **) NULL) ||
      (clip_nexus == (NexusInfo **) NULL))
    ThrowBinaryException(CacheError,"UnableToGetCacheNexus",image->filename);
  p=GetAuthenticPixelCacheNexus(image,nexus_info->region.x,
    nexus_info->region.y,nexus_info->region.width,nexus_info->region.height,
    image_nexus[0],exception);
  indexes=image_nexus[0]->indexes;
  q=nexus_info->pixels;
  nexus_indexes=nexus_info->indexes;
  r=GetVirtualPixelsFromNexus(image->mask,MaskVirtualPixelMethod,
    nexus_info->region.x,nexus_info->region.y,nexus_info->region.width,
    nexus_info->region.height,clip_nexus[0],&image->exception);
  GetMagickPixelPacket(image,&alpha);
  GetMagickPixelPacket(image,&beta);
  number_pixels=(MagickSizeType) nexus_info->region.width*
    nexus_info->region.height;
  for (i=0; i < (ssize_t) number_pixels; i++)
  {
    if ((p == (PixelPacket *) NULL) || (r == (const PixelPacket *) NULL))
      break;
    SetMagickPixelPacket(image,p,indexes+i,&alpha);
    SetMagickPixelPacket(image,q,nexus_indexes+i,&beta);
    MagickPixelCompositeMask(&beta,GetPixelIntensity(image,r),&alpha,
      alpha.opacity,&beta);
    SetPixelRed(q,ClampToQuantum(beta.red));
    SetPixelGreen(q,ClampToQuantum(beta.green));
    SetPixelBlue(q,ClampToQuantum(beta.blue));
    SetPixelOpacity(q,ClampToQuantum(beta.opacity));
    if (cache_info->active_index_channel != MagickFalse)
      SetPixelIndex(nexus_indexes+i,GetPixelIndex(indexes+i));
    p++;
    q++;
    r++;
  }
  clip_nexus=DestroyPixelCacheNexus(clip_nexus,1);
  image_nexus=DestroyPixelCacheNexus(image_nexus,1);
  if (i < (ssize_t) number_pixels)
    return(MagickFalse);
  return(MagickTrue);
}
