static Image *SparseColorOption(const Image *image,const ChannelType channel,
  const SparseColorMethod method,const char *arguments,
  const MagickBooleanType color_from_image,ExceptionInfo *exception)
{
  ChannelType
    channels;

  char
    token[MaxTextExtent];

  const char
    *p;

  double
    *sparse_arguments;

  Image
    *sparse_image;

  MagickBooleanType
    error;

  MagickPixelPacket
    color;

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
   
  channels=channel;
  if (image->colorspace != CMYKColorspace)
    channels=(ChannelType) (channels & ~IndexChannel);   
  if (image->matte == MagickFalse)
    channels=(ChannelType) (channels & ~OpacityChannel);   
  number_colors=0;
  if ((channels & RedChannel) != 0)
    number_colors++;
  if ((channels & GreenChannel) != 0)
    number_colors++;
  if ((channels & BlueChannel) != 0)
    number_colors++;
  if ((channels & IndexChannel) != 0)
    number_colors++;
  if ((channels & OpacityChannel) != 0)
    number_colors++;

   
  p=arguments;
  x=0;
  while( *p != '\0' )
  {
    GetNextToken(p,&p,MaxTextExtent,token);
    if ( token[0] == ',' ) continue;
    if ( isalpha((int) token[0]) || token[0] == '#' ) {
      if ( color_from_image ) {
        (void) ThrowMagickException(exception,GetMagickModule(),
            OptionError, "InvalidArgument", "`%s': %s", "sparse-color",
            "Color arg given, when colors are coming from image");
        return( (Image *) NULL);
      }
      x += number_colors;   
    }
    else {
      x++;    
    }
  }
  error=MagickTrue;
  if ( color_from_image ) {
     
    error = ( x % 2 != 0 ) ? MagickTrue : MagickFalse;
    number_arguments=(x/2)*(2+number_colors);
  }
  else {
     
    error = ( x % (2+number_colors) != 0 ) ? MagickTrue : MagickFalse;
    number_arguments=x;
  }
  if ( error ) {
    (void) ThrowMagickException(exception,GetMagickModule(),
               OptionError, "InvalidArgument", "`%s': %s", "sparse-color",
               "Invalid number of Arguments");
    return( (Image *) NULL);
  }

   
  sparse_arguments=(double *) AcquireQuantumMemory(number_arguments,
    sizeof(*sparse_arguments));
  if (sparse_arguments == (double *) NULL) {
    (void) ThrowMagickException(exception,GetMagickModule(),ResourceLimitError,
      "  MemoryAllocationFailed\n""%s","SparseColorOption");
    return( (Image *) NULL);
  }
  (void) memset(sparse_arguments,0,number_arguments*
    sizeof(*sparse_arguments));
  p=arguments;
  x=0;
  while( *p != '\0' && x < number_arguments ) {
     
    token[0]=',';
    while ( token[0] == ',' )
      GetNextToken(p,&p,MaxTextExtent,token);
    if ( token[0] == '\0' ) break;
    if ( isalpha((int) token[0]) || token[0] == '#' ) {
      (void) ThrowMagickException(exception,GetMagickModule(),
            OptionError, "InvalidArgument", "`%s': %s", "sparse-color",
            "Color found, instead of X-coord");
      error = MagickTrue;
      break;
    }
    sparse_arguments[x++]=StringToDouble(token,(char **) NULL);
     
    token[0]=','; while ( token[0] == ',' ) GetNextToken(p,&p,MaxTextExtent,token);
    if ( token[0] == '\0' ) break;
    if ( isalpha((int) token[0]) || token[0] == '#' ) {
      (void) ThrowMagickException(exception,GetMagickModule(),
            OptionError, "InvalidArgument", "`%s': %s", "sparse-color",
            "Color found, instead of Y-coord");
      error = MagickTrue;
      break;
    }
    sparse_arguments[x++]=StringToDouble(token,(char **) NULL);
     
#if 0
    if ( (color_from_image ) {
       
       
    }
    else
#endif
    {
       
      token[0]=',';
      while ( token[0] == ',' )
        GetNextToken(p,&p,MaxTextExtent,token);
      if ( token[0] == '\0' ) break;
      if ( isalpha((int) token[0]) || token[0] == '#' ) {
         
        (void) QueryMagickColor(token,&color,exception);
        if ( channels & RedChannel )
          sparse_arguments[x++] = QuantumScale*color.red;
        if ( channels & GreenChannel )
          sparse_arguments[x++] = QuantumScale*color.green;
        if ( channels & BlueChannel )
          sparse_arguments[x++] = QuantumScale*color.blue;
        if ( channels & IndexChannel )
          sparse_arguments[x++] = QuantumScale*color.index;
        if ( channels & OpacityChannel )
          sparse_arguments[x++] = QuantumScale*color.opacity;
      }
      else {
         
         
        if ( channels & RedChannel ) {
          while ( token[0] == ',' )
            GetNextToken(p,&p,MaxTextExtent,token);
          if ( token[0] == '\0' || isalpha((int)token[0]) || token[0] == '#' )
            break;
          sparse_arguments[x++]=StringToDouble(token,(char **) NULL);
          token[0] = ',';  
        }
        if ( channels & GreenChannel ) {
          while ( token[0] == ',' )
            GetNextToken(p,&p,MaxTextExtent,token);
          if ( token[0] == '\0' || isalpha((int)token[0]) || token[0] == '#' )
            break;
          sparse_arguments[x++]=StringToDouble(token,(char **) NULL);
          token[0] = ',';  
        }
        if ( channels & BlueChannel ) {
          while ( token[0] == ',' )
            GetNextToken(p,&p,MaxTextExtent,token);
          if ( token[0] == '\0' || isalpha((int)token[0]) || token[0] == '#' )
            break;
          sparse_arguments[x++]=StringToDouble(token,(char **) NULL);
          token[0] = ',';  
        }
        if ( channels & IndexChannel ) {
          while ( token[0] == ',' )
            GetNextToken(p,&p,MaxTextExtent,token);
          if ( token[0] == '\0' || isalpha((int)token[0]) || token[0] == '#' )
            break;
          sparse_arguments[x++]=StringToDouble(token,(char **) NULL);
          token[0] = ',';  
        }
        if ( channels & OpacityChannel ) {
          while ( token[0] == ',' )
            GetNextToken(p,&p,MaxTextExtent,token);
          if ( token[0] == '\0' || isalpha((int)token[0]) || token[0] == '#' )
            break;
          sparse_arguments[x++]=StringToDouble(token,(char **) NULL);
          token[0] = ',';  
        }
      }
    }
  }
  if ( number_arguments != x && !error ) {
    (void) ThrowMagickException(exception,GetMagickModule(),OptionError,
      "  InvalidArgument","`%s': %s","sparse-color","Argument Parsing Error");
    sparse_arguments=(double *) RelinquishMagickMemory(sparse_arguments);
    return( (Image *) NULL);
  }
  if ( error )
    return( (Image *) NULL);

   
  sparse_image=SparseColorImage(image,channels,method,number_arguments,
    sparse_arguments,exception);
  sparse_arguments=(double *) RelinquishMagickMemory(sparse_arguments);
  return( sparse_image );
}
