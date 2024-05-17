static Image *ReadSIXELImage(const ImageInfo *image_info,ExceptionInfo *exception)
{
  char
    *sixel_buffer;

  Image
    *image;

  MagickBooleanType
    status;

  register char
    *p;

  register ssize_t
    x;

  register Quantum
    *q;

  size_t
    length;

  ssize_t
    i,
    j,
    y;

  unsigned char
    *sixel_pixels,
    *sixel_palette;

   
  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickCoreSignature);
  if (image_info->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",
      image_info->filename);
  assert(exception != (ExceptionInfo *) NULL);
  assert(exception->signature == MagickCoreSignature);
  image=AcquireImage(image_info,exception);
  status=OpenBlob(image_info,image,ReadBinaryBlobMode,exception);
  if (status == MagickFalse)
    {
      image=DestroyImageList(image);
      return((Image *) NULL);
    }
   
  length=MagickPathExtent;
  sixel_buffer=(char *) AcquireQuantumMemory((size_t) length+MagickPathExtent,
    sizeof(*sixel_buffer));
  p=sixel_buffer;
  if (sixel_buffer != (char *) NULL)
    while (ReadBlobString(image,p) != (char *) NULL)
    {
      if ((*p == '#') && ((p == sixel_buffer) || (*(p-1) == '\n')))
        continue;
      if ((*p == '}') && (*(p+1) == ';'))
        break;
      p+=strlen(p);
      if ((size_t) (p-sixel_buffer+MagickPathExtent+1) < length)
        continue;
      length<<=1;
      sixel_buffer=(char *) ResizeQuantumMemory(sixel_buffer,length+
        MagickPathExtent+1,sizeof(*sixel_buffer));
      if (sixel_buffer == (char *) NULL)
        break;
      p=sixel_buffer+strlen(sixel_buffer);
    }
