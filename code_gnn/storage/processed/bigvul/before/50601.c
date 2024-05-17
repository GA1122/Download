static const char *GetMagickPropertyLetter(ImageInfo *image_info,
  Image *image,const char letter,ExceptionInfo *exception)
{
#define WarnNoImageReturn(format,arg) \
  if (image == (Image *) NULL ) { \
    (void) ThrowMagickException(exception,GetMagickModule(),OptionWarning, \
      "NoImageForProperty",format,arg); \
    return((const char *) NULL); \
  }
#define WarnNoImageInfoReturn(format,arg) \
  if (image_info == (ImageInfo *) NULL ) { \
    (void) ThrowMagickException(exception,GetMagickModule(),OptionWarning, \
      "NoImageInfoForProperty",format,arg); \
    return((const char *) NULL); \
  }

  char
    value[MagickPathExtent];   

  const char
    *string;      

  if ((image != (Image *) NULL) && (image->debug != MagickFalse))
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  else
    if ((image_info != (ImageInfo *) NULL) &&
        (image_info->debug != MagickFalse))
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s","no-images");
  *value='\0';            
  string=(char *) NULL;   
   
  switch (letter)
  {
    case 'b':   
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      (void) FormatMagickSize(image->extent,MagickFalse,"B",MagickPathExtent,
        value);
      if (image->extent == 0)
        (void) FormatMagickSize(GetBlobSize(image),MagickFalse,"B",
          MagickPathExtent,value);
      break;
    }
    case 'c':   
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      string=GetImageProperty(image,"comment",exception);
      if ( string == (const char *) NULL )
        string="";
      break;
    }
    case 'd':   
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      GetPathComponent(image->magick_filename,HeadPath,value);
      if (*value == '\0')
        string="";
      break;
    }
    case 'e':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      GetPathComponent(image->magick_filename,ExtensionPath,value);
      if (*value == '\0')
        string="";
      break;
    }
    case 'f':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      GetPathComponent(image->magick_filename,TailPath,value);
      if (*value == '\0')
        string="";
      break;
    }
    case 'g':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      (void) FormatLocaleString(value,MagickPathExtent,
        "%.20gx%.20g%+.20g%+.20g",(double) image->page.width,(double)
        image->page.height,(double) image->page.x,(double) image->page.y);
      break;
    }
    case 'h':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      (void) FormatLocaleString(value,MagickPathExtent,"%.20g",(double)
        (image->rows != 0 ? image->rows : image->magick_rows));
      break;
    }
    case 'i':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      string=image->filename;
      break;
    }
    case 'k':  
    {
       
      WarnNoImageReturn("\"%%%c\"",letter);
      (void) FormatLocaleString(value,MagickPathExtent,"%.20g",(double)
        GetNumberColors(image,(FILE *) NULL,exception));
      break;
    }
    case 'l':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      string=GetImageProperty(image,"label",exception);
      if (string == (const char *) NULL)
        string="";
      break;
    }
    case 'm':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      string=image->magick;
      break;
    }
    case 'n':  
    {
      if ( image != (Image *) NULL )
        (void) FormatLocaleString(value,MagickPathExtent,"%.20g",(double)
          GetImageListLength(image));
      else
        string="0";     
      break;
    }
    case 'o':  
      WarnNoImageInfoReturn("\"%%%c\"",letter);
      string=image_info->filename;
      break;
    case 'p':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      (void) FormatLocaleString(value,MagickPathExtent,"%.20g",(double)
        GetImageIndexInList(image));
      break;
    }
    case 'q':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      (void) FormatLocaleString(value,MagickPathExtent,"%.20g",(double)
        MAGICKCORE_QUANTUM_DEPTH);
      break;
    }
    case 'r':  
    {
      ColorspaceType
        colorspace;

      WarnNoImageReturn("\"%%%c\"",letter);
      colorspace=image->colorspace;
      if (SetImageGray(image,exception) != MagickFalse)
        colorspace=GRAYColorspace;    
      (void) FormatLocaleString(value,MagickPathExtent,"%s %s %s",
        CommandOptionToMnemonic(MagickClassOptions,(ssize_t)
        image->storage_class),CommandOptionToMnemonic(MagickColorspaceOptions,
        (ssize_t) colorspace),image->alpha_trait != UndefinedPixelTrait ?
        "Alpha" : "");
      break;
    }
    case 's':  
    {
#if 0   
      if (image_info->number_scenes != 0)
        (void) FormatLocaleString(value,MagickPathExtent,"%.20g",(double)
          image_info->scene);
      else if (image != (Image *) NULL)
        (void) FormatLocaleString(value,MagickPathExtent,"%.20g",(double)
          image->scene);
      else
          string="0";
#else
      WarnNoImageReturn("\"%%%c\"",letter);
      (void) FormatLocaleString(value,MagickPathExtent,"%.20g",(double)
         image->scene);
#endif
      break;
    }
    case 't':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      GetPathComponent(image->magick_filename,BasePath,value);
      if (*value == '\0')
        string="";
      break;
    }
    case 'u':  
    {
      WarnNoImageInfoReturn("\"%%%c\"",letter);
      string=image_info->unique;
      break;
    }
    case 'w':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      (void) FormatLocaleString(value,MagickPathExtent,"%.20g",(double)
        (image->columns != 0 ? image->columns : image->magick_columns));
      break;
    }
    case 'x':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      (void) FormatLocaleString(value,MagickPathExtent,"%.20g",
        fabs(image->resolution.x) > MagickEpsilon ? image->resolution.x : 72.0);
      break;
    }
    case 'y':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      (void) FormatLocaleString(value,MagickPathExtent,"%.20g",
        fabs(image->resolution.y) > MagickEpsilon ? image->resolution.y : 72.0);
      break;
    }
    case 'z':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      (void) FormatLocaleString(value,MagickPathExtent,"%.20g",(double)
        image->depth);
      break;
    }
    case 'A':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      string=CommandOptionToMnemonic(MagickPixelTraitOptions,(ssize_t)
        image->alpha_trait);
      break;
    }
    case 'C':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      string=CommandOptionToMnemonic(MagickCompressOptions,(ssize_t)
        image->compression);
      break;
    }
    case 'D':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      string=CommandOptionToMnemonic(MagickDisposeOptions,(ssize_t)
        image->dispose);
      break;
    }
    case 'G':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      (void) FormatLocaleString(value,MagickPathExtent,"%.20gx%.20g",(double)
        image->magick_columns,(double) image->magick_rows);
      break;
    }
    case 'H':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      (void) FormatLocaleString(value,MagickPathExtent,"%.20g",(double)
        image->page.height);
      break;
    }
    case 'M':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      string=image->magick_filename;
      break;
    }
    case 'O':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      (void) FormatLocaleString(value,MagickPathExtent,"%+ld%+ld",(long)
        image->page.x,(long) image->page.y);
      break;
    }
    case 'P':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      (void) FormatLocaleString(value,MagickPathExtent,"%.20gx%.20g",(double)
        image->page.width,(double) image->page.height);
      break;
    }
    case 'Q':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      (void) FormatLocaleString(value,MagickPathExtent,"%.20g",(double)
        (image->quality == 0 ? 92 : image->quality));
      break;
    }
    case 'S':  
    {
      WarnNoImageInfoReturn("\"%%%c\"",letter);
#if 0  
      if (image_info->number_scenes == 0)
         string="2147483647";
      else if ( image != (Image *) NULL )
        (void) FormatLocaleString(value,MagickPathExtent,"%.20g",(double)
                image_info->scene+image_info->number_scenes);
      else
        string="0";
#else
      (void) FormatLocaleString(value,MagickPathExtent,"%.20g",(double)
        (image_info->number_scenes == 0 ? 2147483647 :
         image_info->number_scenes));
#endif
      break;
    }
    case 'T':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      (void) FormatLocaleString(value,MagickPathExtent,"%.20g",(double)
        image->delay);
      break;
    }
    case 'U':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      string=CommandOptionToMnemonic(MagickResolutionOptions,(ssize_t)
        image->units);
      break;
    }
    case 'W':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      (void) FormatLocaleString(value,MagickPathExtent,"%.20g",(double)
        image->page.width);
      break;
    }
    case 'X':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      (void) FormatLocaleString(value,MagickPathExtent,"%+.20g",(double)
        image->page.x);
      break;
    }
    case 'Y':  
    {
      WarnNoImageReturn("\"%%%c\"",letter);
      (void) FormatLocaleString(value,MagickPathExtent,"%+.20g",(double)
        image->page.y);
      break;
    }
    case '%':  
    {
      string="%";
      break;
    }
    case '@':  
    {
      RectangleInfo
        page;

      WarnNoImageReturn("\"%%%c\"",letter);
      page=GetImageBoundingBox(image,exception);
      (void) FormatLocaleString(value,MagickPathExtent,
        "%.20gx%.20g%+.20g%+.20g",(double) page.width,(double) page.height,
        (double) page.x,(double)page.y);
      break;
    }
    case '#':
    {
       
      WarnNoImageReturn("\"%%%c\"",letter);
      (void) SignatureImage(image,exception);
      string=GetImageProperty(image,"signature",exception);
      break;
    }
  }
  if (string != (char *) NULL)
    return(string);
  if (*value != '\0')
    {
       
      if (image != (Image *) NULL)
        {
          (void) SetImageArtifact(image,"get-property",value);
          return(GetImageArtifact(image,"get-property"));
        }
      else
        {
          (void) SetImageOption(image_info,"get-property",value);
          return(GetImageOption(image_info,"get-property"));
        }
    }
  return((char *) NULL);
}
