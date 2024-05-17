static MagickBooleanType SetCacheAlphaChannel(Image *image,
  const Quantum opacity)
{
  CacheInfo
    *restrict cache_info;

  CacheView
    *restrict image_view;

  MagickBooleanType
    status;

  ssize_t
    y;

  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  assert(image->cache != (Cache) NULL);
  cache_info=(CacheInfo *) image->cache;
  assert(cache_info->signature == MagickSignature);
  image->matte=MagickTrue;
  status=MagickTrue;
  image_view=AcquireVirtualCacheView(image,&image->exception);   
#if defined(MAGICKCORE_OPENMP_SUPPORT)
  #pragma omp parallel for schedule(static,4) shared(status) \
    magick_threads(image,image,1,1)
#endif
  for (y=0; y < (ssize_t) image->rows; y++)
  {
    register PixelPacket
      *restrict q;

    register ssize_t
      x;

    if (status == MagickFalse)
      continue;
    q=GetCacheViewAuthenticPixels(image_view,0,y,image->columns,1,
      &image->exception);
    if (q == (PixelPacket *) NULL)
      {
        status=MagickFalse;
        continue;
      }
    for (x=0; x < (ssize_t) image->columns; x++)
    {
      q->opacity=opacity;
      q++;
    }
    status=SyncCacheViewAuthenticPixels(image_view,&image->exception);
  }
  image_view=DestroyCacheView(image_view);
  return(status);
}
