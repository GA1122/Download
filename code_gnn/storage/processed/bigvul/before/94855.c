MagickExport MagickBooleanType SetImageMask(Image *image,const PixelMask type,
  const Image *mask,ExceptionInfo *exception)
{
  CacheView
    *mask_view,
    *image_view;

  MagickBooleanType
    status;

  ssize_t
    y;

   
  assert(image != (Image *) NULL);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"...");
  assert(image->signature == MagickCoreSignature);
  if (mask == (const Image *) NULL)
    {
      switch (type)
      {
        case WritePixelMask: image->write_mask=MagickFalse; break;
        default: image->read_mask=MagickFalse; break;
      }
      return(SyncImagePixelCache(image,exception));
    }
  switch (type)
  {
    case WritePixelMask: image->write_mask=MagickTrue; break;
    default: image->read_mask=MagickTrue; break;
  }
  if (SyncImagePixelCache(image,exception) == MagickFalse)
    return(MagickFalse);
  status=MagickTrue;
  mask_view=AcquireVirtualCacheView(mask,exception);
  image_view=AcquireAuthenticCacheView(image,exception);
#if defined(MAGICKCORE_OPENMP_SUPPORT)
  #pragma omp parallel for schedule(static,4) shared(status) \
    magick_threads(mask,image,1,1)
#endif
  for (y=0; y < (ssize_t) image->rows; y++)
  {
    register const Quantum
      *magick_restrict p;

    register Quantum
      *magick_restrict q;

    register ssize_t
      x;

    if (status == MagickFalse)
      continue;
    p=GetCacheViewVirtualPixels(mask_view,0,y,mask->columns,1,exception);
    q=GetCacheViewAuthenticPixels(image_view,0,y,image->columns,1,exception);
    if ((p == (const Quantum *) NULL) || (q == (Quantum *) NULL))
      {
        status=MagickFalse;
        continue;
      }
    for (x=0; x < (ssize_t) image->columns; x++)
    {
      MagickRealType
        intensity;

      intensity=GetPixelIntensity(mask,p);
      switch (type)
      {
        case WritePixelMask:
        {
          SetPixelWriteMask(image,ClampToQuantum(QuantumRange-intensity),q);
          break;
        }
        default:
        {
          SetPixelReadMask(image,ClampToQuantum(QuantumRange-intensity),q);
          break;
        }
      }
      p+=GetPixelChannels(mask);
      q+=GetPixelChannels(image);
    }
    if (SyncCacheViewAuthenticPixels(image_view,exception) == MagickFalse)
      status=MagickFalse;
  }
  mask_view=DestroyCacheView(mask_view);
  image_view=DestroyCacheView(image_view);
  return(status);
}