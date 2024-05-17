static Image *SparseColorOption(const Image *image,
  const SparseColorMethod method,const char *arguments,ExceptionInfo *exception)
{
  char
    token[MagickPathExtent];

  const char
    *p;

  double
    *sparse_arguments;

  Image
    *sparse_image;

  PixelInfo
    color;

  MagickBooleanType
    error;

  register size_t
    x;

  size_t
    number_arguments,
    number_colors;

  assert(image != (Image *) NULL);
  assert(image->signature == MagickCoreSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  assert(exception != (ExceptionInfo *) NULL);
  assert(exception->signature == MagickCoreSignature);
   
  number_colors=0;
  if ((GetPixelRedTraits(image) & UpdatePixelTrait) != 0)
    number_colors++;
  if ((GetPixelGreenTraits(image) & UpdatePixelTrait) != 0)
    number_colors++;
  if ((GetPixelBlueTraits(image) & UpdatePixelTrait) != 0)
    number_colors++;
  if (((GetPixelBlackTraits(image) & UpdatePixelTrait) != 0) &&
      (image->colorspace == CMYKColorspace))
    number_colors++;
  if (((GetPixelAlphaTraits(image) & UpdatePixelTrait) != 0) &&
      image->alpha_trait != UndefinedPixelTrait)
    number_colors++;

   
  p=arguments;
  x=0;
  while( *p != '\0' )
  {
    GetNextToken(p,&p,MagickPathExtent,token);
    if ( token[0] == ',' ) continue;
    if ( isalpha((int) token[0]) || token[0] == '#' )
      x += number_colors;   
    else
      x++;    
  }
   
  if ((x % (2+number_colors)) != 0)
    {
      (void) ThrowMagickException(exception,GetMagickModule(),OptionError,
        "InvalidArgument","'%s': %s", "sparse-color",
        "Invalid number of Arguments");
      return( (Image *) NULL);
    }
  error=MagickFalse;
  number_arguments=x;

   
  sparse_arguments=(double *) AcquireQuantumMemory(number_arguments,
    sizeof(*sparse_arguments));
  if (sparse_arguments == (double *) NULL) {
    (void) ThrowMagickException(exception,GetMagickModule(),ResourceLimitError,
      "MemoryAllocationFailed","%s","SparseColorOption");
    return( (Image *) NULL);
  }
  (void) memset(sparse_arguments,0,number_arguments*
    sizeof(*sparse_arguments));
  p=arguments;
  x=0;
  while( *p != '\0' && x < number_arguments ) {
     
    token[0]=','; while ( token[0] == ',' ) GetNextToken(p,&p,MagickPathExtent,token);
    if ( token[0] == '\0' ) break;
    if ( isalpha((int) token[0]) || token[0] == '#' ) {
      (void) ThrowMagickException(exception,GetMagickModule(),
            OptionError, "InvalidArgument", "'%s': %s", "sparse-color",
            "Color found, instead of X-coord");
      error=MagickTrue;
      break;
    }
    sparse_arguments[x++]=StringToDouble(token,(char **) NULL);
     
    token[0]=','; while ( token[0] == ',' ) GetNextToken(p,&p,MagickPathExtent,token);
    if ( token[0] == '\0' ) break;
    if ( isalpha((int) token[0]) || token[0] == '#' ) {
      (void) ThrowMagickException(exception,GetMagickModule(),
            OptionError, "InvalidArgument", "'%s': %s", "sparse-color",
            "Color found, instead of Y-coord");
      error=MagickTrue;
      break;
    }
    sparse_arguments[x++]=StringToDouble(token,(char **) NULL);
     
    token[0]=','; while ( token[0] == ',' ) GetNextToken(p,&p,MagickPathExtent,token);
    if ( token[0] == '\0' ) break;
    if ( isalpha((int) token[0]) || token[0] == '#' ) {
       
      (void) QueryColorCompliance(token,AllCompliance,&color,
                exception);
      if ((GetPixelRedTraits(image) & UpdatePixelTrait) != 0)
        sparse_arguments[x++] = QuantumScale*color.red;
      if ((GetPixelGreenTraits(image) & UpdatePixelTrait) != 0)
        sparse_arguments[x++] = QuantumScale*color.green;
      if ((GetPixelBlueTraits(image) & UpdatePixelTrait) != 0)
        sparse_arguments[x++] = QuantumScale*color.blue;
      if (((GetPixelBlackTraits(image) & UpdatePixelTrait) != 0) &&
          (image->colorspace == CMYKColorspace))
        sparse_arguments[x++] = QuantumScale*color.black;
      if (((GetPixelAlphaTraits(image) & UpdatePixelTrait) != 0) &&
          image->alpha_trait != UndefinedPixelTrait)
        sparse_arguments[x++] = QuantumScale*color.alpha;
    }
    else {
       
       
      if ((GetPixelRedTraits(image) & UpdatePixelTrait) != 0)
        {
        while ( token[0] == ',' ) GetNextToken(p,&p,MagickPathExtent,token);
        if ( token[0] == '\0' || isalpha((int)token[0]) || token[0] == '#' )
          break;
        sparse_arguments[x++]=StringToDouble(token,(char **) NULL);
        token[0] = ',';  
      }
      if ((GetPixelGreenTraits(image) & UpdatePixelTrait) != 0)
        {
        while ( token[0] == ',' ) GetNextToken(p,&p,MagickPathExtent,token);
        if ( token[0] == '\0' || isalpha((int)token[0]) || token[0] == '#' )
          break;
        sparse_arguments[x++]=StringToDouble(token,(char **) NULL);
        token[0] = ',';  
      }
      if ((GetPixelBlueTraits(image) & UpdatePixelTrait) != 0)
        {
        while ( token[0] == ',' ) GetNextToken(p,&p,MagickPathExtent,token);
        if ( token[0] == '\0' || isalpha((int)token[0]) || token[0] == '#' )
          break;
        sparse_arguments[x++]=StringToDouble(token,(char **) NULL);
        token[0] = ',';  
      }
      if (((GetPixelBlackTraits(image) & UpdatePixelTrait) != 0) &&
          (image->colorspace == CMYKColorspace))
        {
        while ( token[0] == ',' ) GetNextToken(p,&p,MagickPathExtent,token);
        if ( token[0] == '\0' || isalpha((int)token[0]) || token[0] == '#' )
          break;
        sparse_arguments[x++]=StringToDouble(token,(char **) NULL);
        token[0] = ',';  
      }
      if (((GetPixelAlphaTraits(image) & UpdatePixelTrait) != 0) &&
          image->alpha_trait != UndefinedPixelTrait)
        {
        while ( token[0] == ',' ) GetNextToken(p,&p,MagickPathExtent,token);
        if ( token[0] == '\0' || isalpha((int)token[0]) || token[0] == '#' )
          break;
        sparse_arguments[x++]=StringToDouble(token,(char **) NULL);
        token[0] = ',';  
      }
    }
  }
  if (error != MagickFalse)
    {
      sparse_arguments=(double *) RelinquishMagickMemory(sparse_arguments);
      return((Image *) NULL);
    }
  if (number_arguments != x)
    {
      sparse_arguments=(double *) RelinquishMagickMemory(sparse_arguments);
      (void) ThrowMagickException(exception,GetMagickModule(),OptionError,
        "InvalidArgument","'%s': %s","sparse-color","Argument Parsing Error");
      return((Image *) NULL);
    }
   
  sparse_image=SparseColorImage(image,method,number_arguments,sparse_arguments,
    exception);
  sparse_arguments=(double *) RelinquishMagickMemory(sparse_arguments);
  return( sparse_image );
}