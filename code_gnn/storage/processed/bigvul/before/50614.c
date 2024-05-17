MagickExport MagickBooleanType SetImageProperty(Image *image,
  const char *property,const char *value,ExceptionInfo *exception)
{
  MagickBooleanType
    status;

  MagickStatusType
    flags;

  assert(image != (Image *) NULL);
  assert(image->signature == MagickCoreSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  if (image->properties == (void *) NULL)
    image->properties=NewSplayTree(CompareSplayTreeString,
      RelinquishMagickMemory,RelinquishMagickMemory);   
  if (value == (const char *) NULL)
    return(DeleteImageProperty(image,property));   
  status=MagickTrue;
  if (strlen(property) <= 1)
    {
       
      (void) ThrowMagickException(exception,GetMagickModule(),OptionError,
        "SetReadOnlyProperty","`%s'",property);
      return(MagickFalse);
    }

   
   
  switch (*property)
  {
#if 0   
    case '8':
    {
      if (LocaleNCompare("8bim:",property,5) == 0)
        {
          (void) ThrowMagickException(exception,GetMagickModule(),OptionError,
            "SetReadOnlyProperty","`%s'",property);
          return(MagickFalse);
        }
      break;
    }
#endif
    case 'B':
    case 'b':
    {
      if (LocaleCompare("background",property) == 0)
        {
          (void) QueryColorCompliance(value,AllCompliance,
               &image->background_color,exception);
           
           
        }
      break;  
    }
    case 'C':
    case 'c':
    {
      if (LocaleCompare("channels",property) == 0)
        {
          (void) ThrowMagickException(exception,GetMagickModule(),OptionError,
            "SetReadOnlyProperty","`%s'",property);
          return(MagickFalse);
        }
      if (LocaleCompare("colorspace",property) == 0)
        {
          ssize_t
            colorspace;

          colorspace=ParseCommandOption(MagickColorspaceOptions,MagickFalse,
            value);
          if (colorspace < 0)
            return(MagickFalse);  
          return(SetImageColorspace(image,(ColorspaceType) colorspace,exception));
        }
      if (LocaleCompare("compose",property) == 0)
        {
          ssize_t
            compose;

          compose=ParseCommandOption(MagickComposeOptions,MagickFalse,value);
          if (compose < 0)
            return(MagickFalse);  
          image->compose=(CompositeOperator) compose;
          return(MagickTrue);
        }
      if (LocaleCompare("compress",property) == 0)
        {
          ssize_t
            compression;

          compression=ParseCommandOption(MagickCompressOptions,MagickFalse,
            value);
          if (compression < 0)
            return(MagickFalse);  
          image->compression=(CompressionType) compression;
          return(MagickTrue);
        }
      break;  
    }
    case 'D':
    case 'd':
    {
      if (LocaleCompare("delay",property) == 0)
        {
          GeometryInfo
            geometry_info;

          flags=ParseGeometry(value,&geometry_info);
          if ((flags & GreaterValue) != 0)
            {
              if (image->delay > (size_t) floor(geometry_info.rho+0.5))
                image->delay=(size_t) floor(geometry_info.rho+0.5);
            }
          else
            if ((flags & LessValue) != 0)
              {
                if (image->delay < (size_t) floor(geometry_info.rho+0.5))
                  image->delay=(ssize_t)
                    floor(geometry_info.sigma+0.5);
              }
            else
              image->delay=(size_t) floor(geometry_info.rho+0.5);
          if ((flags & SigmaValue) != 0)
            image->ticks_per_second=(ssize_t) floor(geometry_info.sigma+0.5);
          return(MagickTrue);
        }
      if (LocaleCompare("delay_units",property) == 0)
        {
          (void) ThrowMagickException(exception,GetMagickModule(),OptionError,
            "SetReadOnlyProperty","`%s'",property);
          return(MagickFalse);
        }
      if (LocaleCompare("density",property) == 0)
        {
          GeometryInfo
            geometry_info;

          flags=ParseGeometry(value,&geometry_info);
          image->resolution.x=geometry_info.rho;
          image->resolution.y=geometry_info.sigma;
          if ((flags & SigmaValue) == 0)
            image->resolution.y=image->resolution.x;
          return(MagickTrue);
        }
      if (LocaleCompare("depth",property) == 0)
        {
          image->depth=StringToUnsignedLong(value);
          return(MagickTrue);
        }
      if (LocaleCompare("dispose",property) == 0)
        {
          ssize_t
            dispose;

          dispose=ParseCommandOption(MagickDisposeOptions,MagickFalse,value);
          if (dispose < 0)
            return(MagickFalse);  
          image->dispose=(DisposeType) dispose;
          return(MagickTrue);
        }
      break;  
    }
#if 0   
    case 'E':
    case 'e':
    {
      if (LocaleNCompare("exif:",property,5) == 0)
        {
          (void) ThrowMagickException(exception,GetMagickModule(),OptionError,
            "SetReadOnlyProperty","`%s'",property);
          return(MagickFalse);
        }
      break;  
    }
    case 'F':
    case 'f':
    {
      if (LocaleNCompare("fx:",property,3) == 0)
        {
          (void) ThrowMagickException(exception,GetMagickModule(),OptionError,
            "SetReadOnlyProperty","`%s'",property);
          return(MagickFalse);
        }
      break;  
    }
#endif
    case 'G':
    case 'g':
    {
      if (LocaleCompare("gamma",property) == 0)
        {
          image->gamma=StringToDouble(value,(char **) NULL);
          return(MagickTrue);
        }
      if (LocaleCompare("gravity",property) == 0)
        {
          ssize_t
            gravity;

          gravity=ParseCommandOption(MagickGravityOptions,MagickFalse,value);
          if (gravity < 0)
            return(MagickFalse);  
          image->gravity=(GravityType) gravity;
          return(MagickTrue);
        }
      break;  
    }
    case 'H':
    case 'h':
    {
      if (LocaleCompare("height",property) == 0)
        {
          (void) ThrowMagickException(exception,GetMagickModule(),OptionError,
            "SetReadOnlyProperty","`%s'",property);
          return(MagickFalse);
        }
      break;  
    }
    case 'I':
    case 'i':
    {
      if (LocaleCompare("intensity",property) == 0)
        {
          ssize_t
            intensity;

          intensity=ParseCommandOption(MagickIntentOptions,MagickFalse,value);
          if (intensity < 0)
            return(MagickFalse);
          image->intensity=(PixelIntensityMethod) intensity;
          return(MagickTrue);
        }
      if (LocaleCompare("intent",property) == 0)
        {
          ssize_t
            rendering_intent;

          rendering_intent=ParseCommandOption(MagickIntentOptions,MagickFalse,
            value);
          if (rendering_intent < 0)
            return(MagickFalse);  
          image->rendering_intent=(RenderingIntent) rendering_intent;
          return(MagickTrue);
        }
      if (LocaleCompare("interpolate",property) == 0)
        {
          ssize_t
            interpolate;

          interpolate=ParseCommandOption(MagickInterpolateOptions,MagickFalse,
            value);
          if (interpolate < 0)
            return(MagickFalse);  
          image->interpolate=(PixelInterpolateMethod) interpolate;
          return(MagickTrue);
        }
#if 0   
      if (LocaleNCompare("iptc:",property,5) == 0)
        {
          (void) ThrowMagickException(exception,GetMagickModule(),OptionError,
            "SetReadOnlyProperty","`%s'",property);
          return(MagickFalse);
        }
#endif
      break;  
    }
    case 'K':
    case 'k':
      if (LocaleCompare("kurtosis",property) == 0)
        {
          (void) ThrowMagickException(exception,GetMagickModule(),OptionError,
            "SetReadOnlyProperty","`%s'",property);
          return(MagickFalse);
        }
      break;  
    case 'L':
    case 'l':
    {
      if (LocaleCompare("loop",property) == 0)
        {
          image->iterations=StringToUnsignedLong(value);
          return(MagickTrue);
        }
      break;  
    }
    case 'M':
    case 'm':
      if ((LocaleCompare("magick",property) == 0) ||
          (LocaleCompare("max",property) == 0) ||
          (LocaleCompare("mean",property) == 0) ||
          (LocaleCompare("min",property) == 0) ||
          (LocaleCompare("min",property) == 0))
        {
          (void) ThrowMagickException(exception,GetMagickModule(),OptionError,
             "SetReadOnlyProperty","`%s'",property);
          return(MagickFalse);
        }
      break;  
    case 'O':
    case 'o':
      if (LocaleCompare("opaque",property) == 0)
        {
          (void) ThrowMagickException(exception,GetMagickModule(),OptionError,
            "SetReadOnlyProperty","`%s'",property);
          return(MagickFalse);
        }
      break;  
    case 'P':
    case 'p':
    {
      if (LocaleCompare("page",property) == 0)
        {
          char
            *geometry;

          geometry=GetPageGeometry(value);
          flags=ParseAbsoluteGeometry(geometry,&image->page);
          geometry=DestroyString(geometry);
          return(MagickTrue);
        }
#if 0   
      if (LocaleNCompare("pixel:",property,6) == 0)
        {
          (void) ThrowMagickException(exception,GetMagickModule(),OptionError,
            "SetReadOnlyProperty","`%s'",property);
          return(MagickFalse);
        }
#endif
      if (LocaleCompare("profile",property) == 0)
        {
          ImageInfo
            *image_info;

          StringInfo
            *profile;

          image_info=AcquireImageInfo();
          (void) CopyMagickString(image_info->filename,value,MagickPathExtent);
          (void) SetImageInfo(image_info,1,exception);
          profile=FileToStringInfo(image_info->filename,~0UL,exception);
          if (profile != (StringInfo *) NULL)
            status=SetImageProfile(image,image_info->magick,profile,exception);
          image_info=DestroyImageInfo(image_info);
          return(MagickTrue);
        }
      break;  
    }
    case 'R':
    case 'r':
    {
      if (LocaleCompare("rendering-intent",property) == 0)
        {
          ssize_t
            rendering_intent;

          rendering_intent=ParseCommandOption(MagickIntentOptions,MagickFalse,
            value);
          if (rendering_intent < 0)
            return(MagickFalse);  
          image->rendering_intent=(RenderingIntent) rendering_intent;
          return(MagickTrue);
        }
      break;  
    }
    case 'S':
    case 's':
      if ((LocaleCompare("size",property) == 0) ||
          (LocaleCompare("skewness",property) == 0) ||
          (LocaleCompare("scenes",property) == 0) ||
          (LocaleCompare("standard-deviation",property) == 0))
        {
          (void) ThrowMagickException(exception,GetMagickModule(),OptionError,
            "SetReadOnlyProperty","`%s'",property);
          return(MagickFalse);
        }
      break;  
    case 'T':
    case 't':
    {
      if (LocaleCompare("tile-offset",property) == 0)
        {
          char
            *geometry;

          geometry=GetPageGeometry(value);
          flags=ParseAbsoluteGeometry(geometry,&image->tile_offset);
          geometry=DestroyString(geometry);
          return(MagickTrue);
        }
      break;  
    }
    case 'U':
    case 'u':
    {
      if (LocaleCompare("units",property) == 0)
        {
          ssize_t
            units;

          units=ParseCommandOption(MagickResolutionOptions,MagickFalse,value);
          if (units < 0)
            return(MagickFalse);  
          image->units=(ResolutionType) units;
          return(MagickTrue);
        }
      break;  
    }
    case 'V':
    case 'v':
    {
      if (LocaleCompare("version",property) == 0)
        {
          (void) ThrowMagickException(exception,GetMagickModule(),OptionError,
            "SetReadOnlyProperty","`%s'",property);
          return(MagickFalse);
        }
      break;  
    }
    case 'W':
    case 'w':
    {
      if (LocaleCompare("width",property) == 0)
        {
          (void) ThrowMagickException(exception,GetMagickModule(),OptionError,
            "SetReadOnlyProperty","`%s'",property);
          return(MagickFalse);
        }
      break;  
    }
#if 0   
    case 'X':
    case 'x':
    {
      if (LocaleNCompare("xmp:",property,4) == 0)
        {
          (void) ThrowMagickException(exception,GetMagickModule(),OptionError,
            "SetReadOnlyProperty","`%s'",property);
          return(MagickFalse);
        }
      break;  
    }
#endif
  }
   
  status=AddValueToSplayTree((SplayTreeInfo *) image->properties,
    ConstantString(property),ConstantString(value));
   
  return(status);
}
