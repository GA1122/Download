MagickExport MagickBooleanType OrderedPosterizeImageChannel(Image *image,
  const ChannelType channel,const char *threshold_map,ExceptionInfo *exception)
{
#define DitherImageTag  "Dither/Image"

  CacheView
    *image_view;

  LongPixelPacket
    levels;

  MagickBooleanType
    status;

  MagickOffsetType
    progress;

  ssize_t
    y;

  ThresholdMap
    *map;

  assert(image != (Image *) NULL);
  assert(image->signature == MagickCoreSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  assert(exception != (ExceptionInfo *) NULL);
  assert(exception->signature == MagickCoreSignature);
  if (threshold_map == (const char *) NULL)
    return(MagickTrue);
  {
    char
      token[MaxTextExtent];

    register const char
      *p;

    p=(char *)threshold_map;
    while (((isspace((int) ((unsigned char) *p)) != 0) || (*p == ',')) &&
                    (*p != '\0'))
      p++;
    threshold_map=p;
    while (((isspace((int) ((unsigned char) *p)) == 0) && (*p != ',')) &&
                    (*p != '\0')) {
      if ((p-threshold_map) >= (MaxTextExtent-1))
        break;
      token[p-threshold_map] = *p;
      p++;
    }
    token[p-threshold_map] = '\0';
    map = GetThresholdMap(token, exception);
    if ( map == (ThresholdMap *) NULL ) {
      (void) ThrowMagickException(exception,GetMagickModule(),OptionError,
        "InvalidArgument","%s : '%s'","ordered-dither",threshold_map);
      return(MagickFalse);
    }
  }
   
#if 1
  {  
    char *p;

    p = strchr((char *) threshold_map,',');
    if ( p != (char *) NULL && isdigit((int) ((unsigned char) *(++p))) )
      levels.index = (unsigned int) strtoul(p, &p, 10);
    else
      levels.index = 2;

    levels.red     = ((channel & RedChannel  )   != 0) ? levels.index : 0;
    levels.green   = ((channel & GreenChannel)   != 0) ? levels.index : 0;
    levels.blue    = ((channel & BlueChannel)    != 0) ? levels.index : 0;
    levels.opacity = ((channel & OpacityChannel) != 0) ? levels.index : 0;
    levels.index   = ((channel & IndexChannel)   != 0
            && (image->colorspace == CMYKColorspace)) ? levels.index : 0;

     
    if ( p != (char *) NULL && *p == ',' ) {
      p=strchr((char *) threshold_map,',');
      p++;
      if ((channel & RedChannel) != 0)
        levels.red = (unsigned int) strtoul(p, &p, 10),   (void)(*p == ',' && p++);
      if ((channel & GreenChannel) != 0)
        levels.green = (unsigned int) strtoul(p, &p, 10), (void)(*p == ',' && p++);
      if ((channel & BlueChannel) != 0)
        levels.blue = (unsigned int) strtoul(p, &p, 10),  (void)(*p == ',' && p++);
      if ((channel & IndexChannel) != 0 && image->colorspace == CMYKColorspace)
        levels.index=(unsigned int) strtoul(p, &p, 10), (void)(*p == ',' && p++);
      if ((channel & OpacityChannel) != 0)
        levels.opacity = (unsigned int) strtoul(p, &p, 10), (void)(*p == ',' && p++);
    }
  }
#else
   
   
#endif

#if 0
printf("DEBUG levels  r=%u g=%u b=%u a=%u i=%u\n",
     levels.red, levels.green, levels.blue, levels.opacity, levels.index);
#endif

  {  
    ssize_t
      d;

     
    d = map->divisor-1;

     
    levels.red     = levels.red     ? levels.red-1     : 0;
    levels.green   = levels.green   ? levels.green-1   : 0;
    levels.blue    = levels.blue    ? levels.blue-1    : 0;
    levels.opacity = levels.opacity ? levels.opacity-1 : 0;
    levels.index   = levels.index   ? levels.index-1   : 0;

    if (SetImageStorageClass(image,DirectClass) == MagickFalse)
      {
        InheritException(exception,&image->exception);
        return(MagickFalse);
      }
    status=MagickTrue;
    progress=0;
    image_view=AcquireAuthenticCacheView(image,exception);
#if defined(MAGICKCORE_OPENMP_SUPPORT)
    #pragma omp parallel for schedule(static) shared(progress,status) \
      magick_number_threads(image,image,image->rows,1)
#endif
    for (y=0; y < (ssize_t) image->rows; y++)
    {
      register IndexPacket
        *magick_restrict indexes;

      register ssize_t
        x;

      register PixelPacket
        *magick_restrict q;

      if (status == MagickFalse)
        continue;
      q=GetCacheViewAuthenticPixels(image_view,0,y,image->columns,1,exception);
      if (q == (PixelPacket *) NULL)
        {
          status=MagickFalse;
          continue;
        }
      indexes=GetCacheViewAuthenticIndexQueue(image_view);
      for (x=0; x < (ssize_t) image->columns; x++)
      {
        register ssize_t
          threshold,
          t,
          l;

         
        threshold = map->levels[(x%map->width) +map->width*(y%map->height)];

         
        if (levels.red) {
          t = (ssize_t) (QuantumScale*GetPixelRed(q)*(levels.red*d+1));
          l = t/d;  t = t-l*d;
          SetPixelRed(q,ClampToQuantum((MagickRealType)
            ((l+(t >= threshold))*(MagickRealType) QuantumRange/levels.red)));
        }
        if (levels.green) {
          t = (ssize_t) (QuantumScale*GetPixelGreen(q)*
            (levels.green*d+1));
          l = t/d;  t = t-l*d;
          SetPixelGreen(q,ClampToQuantum((MagickRealType)
            ((l+(t >= threshold))*(MagickRealType) QuantumRange/levels.green)));
        }
        if (levels.blue) {
          t = (ssize_t) (QuantumScale*GetPixelBlue(q)*
            (levels.blue*d+1));
          l = t/d;  t = t-l*d;
          SetPixelBlue(q,ClampToQuantum((MagickRealType)
            ((l+(t >= threshold))*(MagickRealType) QuantumRange/levels.blue)));
        }
        if (levels.opacity) {
          t = (ssize_t) ((1.0-QuantumScale*GetPixelOpacity(q))*
            (levels.opacity*d+1));
          l = t/d;  t = t-l*d;
          SetPixelOpacity(q,ClampToQuantum((MagickRealType)
            ((1.0-l-(t >= threshold))*(MagickRealType) QuantumRange/
            levels.opacity)));
        }
        if (levels.index) {
          t = (ssize_t) (QuantumScale*GetPixelIndex(indexes+x)*
            (levels.index*d+1));
          l = t/d;  t = t-l*d;
          SetPixelIndex(indexes+x,ClampToQuantum((MagickRealType) ((l+
            (t>=threshold))*(MagickRealType) QuantumRange/levels.index)));
        }
        q++;
      }
      if (SyncCacheViewAuthenticPixels(image_view,exception) == MagickFalse)
        status=MagickFalse;
      if (image->progress_monitor != (MagickProgressMonitor) NULL)
        {
          MagickBooleanType
            proceed;

#if defined(MAGICKCORE_OPENMP_SUPPORT)
          #pragma omp atomic
#endif
          progress++;
          proceed=SetImageProgress(image,DitherImageTag,progress,image->rows);
          if (proceed == MagickFalse)
            status=MagickFalse;
        }
    }
    image_view=DestroyCacheView(image_view);
  }
  map=DestroyThresholdMap(map);
  return(MagickTrue);
}
