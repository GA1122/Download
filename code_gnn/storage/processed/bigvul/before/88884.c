static MagickBooleanType ForwardFourierTransform(FourierInfo *fourier_info,
  const Image *image,double *magnitude_pixels,double *phase_pixels,
  ExceptionInfo *exception)
{
  CacheView
    *image_view;

  const char
    *value;

  double
    *source_pixels;

  fftw_complex
    *forward_pixels;

  fftw_plan
    fftw_r2c_plan;

  MemoryInfo
    *forward_info,
    *source_info;

  register const IndexPacket
    *indexes;

  register const PixelPacket
    *p;

  register ssize_t
    i,
    x;

  ssize_t
    y;

   
  source_info=AcquireVirtualMemory((size_t) fourier_info->width,
    fourier_info->height*sizeof(*source_pixels));
  if (source_info == (MemoryInfo *) NULL)
    {
      (void) ThrowMagickException(exception,GetMagickModule(),
        ResourceLimitError,"MemoryAllocationFailed","`%s'",image->filename);
      return(MagickFalse);
    }
  source_pixels=(double *) GetVirtualMemoryBlob(source_info);
  memset(source_pixels,0,fourier_info->width*fourier_info->height*
    sizeof(*source_pixels));
  i=0L;
  image_view=AcquireVirtualCacheView(image,exception);
  for (y=0L; y < (ssize_t) fourier_info->height; y++)
  {
    p=GetCacheViewVirtualPixels(image_view,0L,y,fourier_info->width,1UL,
      exception);
    if (p == (const PixelPacket *) NULL)
      break;
    indexes=GetCacheViewVirtualIndexQueue(image_view);
    for (x=0L; x < (ssize_t) fourier_info->width; x++)
    {
      switch (fourier_info->channel)
      {
        case RedChannel:
        default:
        {
          source_pixels[i]=QuantumScale*GetPixelRed(p);
          break;
        }
        case GreenChannel:
        {
          source_pixels[i]=QuantumScale*GetPixelGreen(p);
          break;
        }
        case BlueChannel:
        {
          source_pixels[i]=QuantumScale*GetPixelBlue(p);
          break;
        }
        case OpacityChannel:
        {
          source_pixels[i]=QuantumScale*GetPixelOpacity(p);
          break;
        }
        case IndexChannel:
        {
          source_pixels[i]=QuantumScale*GetPixelIndex(indexes+x);
          break;
        }
        case GrayChannels:
        {
          source_pixels[i]=QuantumScale*GetPixelGray(p);
          break;
        }
      }
      i++;
      p++;
    }
  }
  image_view=DestroyCacheView(image_view);
  forward_info=AcquireVirtualMemory((size_t) fourier_info->width,
    (fourier_info->height/2+1)*sizeof(*forward_pixels));
  if (forward_info == (MemoryInfo *) NULL)
    {
      (void) ThrowMagickException(exception,GetMagickModule(),
        ResourceLimitError,"MemoryAllocationFailed","`%s'",image->filename);
      source_info=(MemoryInfo *) RelinquishVirtualMemory(source_info);
      return(MagickFalse);
    }
  forward_pixels=(fftw_complex *) GetVirtualMemoryBlob(forward_info);
#if defined(MAGICKCORE_OPENMP_SUPPORT)
  #pragma omp critical (MagickCore_ForwardFourierTransform)
#endif
  fftw_r2c_plan=fftw_plan_dft_r2c_2d(fourier_info->width,fourier_info->height,
    source_pixels,forward_pixels,FFTW_ESTIMATE);
  fftw_execute_dft_r2c(fftw_r2c_plan,source_pixels,forward_pixels);
  fftw_destroy_plan(fftw_r2c_plan);
  source_info=(MemoryInfo *) RelinquishVirtualMemory(source_info);
  value=GetImageArtifact(image,"fourier:normalize");
  if ((value == (const char *) NULL) || (LocaleCompare(value,"forward") == 0))
    {
      double
        gamma;

       
      i=0L;
      gamma=PerceptibleReciprocal((double) fourier_info->width*
        fourier_info->height);
      for (y=0L; y < (ssize_t) fourier_info->height; y++)
        for (x=0L; x < (ssize_t) fourier_info->center; x++)
        {
#if defined(MAGICKCORE_HAVE_COMPLEX_H)
          forward_pixels[i]*=gamma;
#else
          forward_pixels[i][0]*=gamma;
          forward_pixels[i][1]*=gamma;
#endif
          i++;
        }
    }
   
  i=0L;
  if (fourier_info->modulus != MagickFalse)
    for (y=0L; y < (ssize_t) fourier_info->height; y++)
      for (x=0L; x < (ssize_t) fourier_info->center; x++)
      {
        magnitude_pixels[i]=cabs(forward_pixels[i]);
        phase_pixels[i]=carg(forward_pixels[i]);
        i++;
      }
  else
    for (y=0L; y < (ssize_t) fourier_info->height; y++)
      for (x=0L; x < (ssize_t) fourier_info->center; x++)
      {
        magnitude_pixels[i]=creal(forward_pixels[i]);
        phase_pixels[i]=cimag(forward_pixels[i]);
        i++;
      }
  forward_info=(MemoryInfo *) RelinquishVirtualMemory(forward_info);
  return(MagickTrue);
}