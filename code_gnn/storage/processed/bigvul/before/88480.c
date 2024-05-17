static MagickBooleanType WriteMATImage(const ImageInfo *image_info,Image *image,
  ExceptionInfo *exception)
{
  char
    MATLAB_HDR[0x80];

  MagickBooleanType
    status;

  MagickOffsetType
    scene;

  size_t
    imageListLength;

  struct tm
    utc_time;

  time_t
    current_time;

   
  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickCoreSignature);
  assert(image != (Image *) NULL);
  assert(image->signature == MagickCoreSignature);
  (void) LogMagickEvent(CoderEvent,GetMagickModule(),"enter MAT");
  assert(exception != (ExceptionInfo *) NULL);
  assert(exception->signature == MagickCoreSignature);
  status=OpenBlob(image_info,image,WriteBinaryBlobMode,exception);
  if (status == MagickFalse)
    return(MagickFalse);
  image->depth=8;

  current_time=GetMagickTime();
  GetMagickUTCtime(&current_time,&utc_time);
  (void) memset(MATLAB_HDR,' ',MagickMin(sizeof(MATLAB_HDR),124));
  FormatLocaleString(MATLAB_HDR,sizeof(MATLAB_HDR),
    "MATLAB 5.0 MAT-file, Platform: %s, Created on: %s %s %2d %2d:%2d:%2d %d",
    OsDesc,DayOfWTab[utc_time.tm_wday],MonthsTab[utc_time.tm_mon],
    utc_time.tm_mday,utc_time.tm_hour,utc_time.tm_min,
    utc_time.tm_sec,utc_time.tm_year+1900);
  MATLAB_HDR[0x7C]=0;
  MATLAB_HDR[0x7D]=1;
  MATLAB_HDR[0x7E]='I';
  MATLAB_HDR[0x7F]='M';
  (void) WriteBlob(image,sizeof(MATLAB_HDR),(unsigned char *) MATLAB_HDR);
  scene=0;
  imageListLength=GetImageListLength(image);
  do
  {
    char
      padding;

    MagickBooleanType
      is_gray;

    QuantumInfo
      *quantum_info;

    size_t
      data_size;

    unsigned char
      *pixels;

    unsigned int
      z;

    (void) TransformImageColorspace(image,sRGBColorspace,exception);
    is_gray=SetImageGray(image,exception);
    z=(is_gray != MagickFalse) ? 0 : 3;

     
    data_size = image->rows * image->columns;
    if (is_gray == MagickFalse)
      data_size*=3;
    padding=((unsigned char)(data_size-1) & 0x7) ^ 0x7;

    (void) WriteBlobLSBLong(image,miMATRIX);
    (void) WriteBlobLSBLong(image,(unsigned int) data_size+padding+
      ((is_gray != MagickFalse) ? 48 : 56));
    (void) WriteBlobLSBLong(image,0x6);  
    (void) WriteBlobLSBLong(image,0x8);  
    (void) WriteBlobLSBLong(image,0x6);  
    (void) WriteBlobLSBLong(image,0);
    (void) WriteBlobLSBLong(image,0x5);  
    (void) WriteBlobLSBLong(image,(is_gray != MagickFalse) ? 0x8 : 0xC);  
    (void) WriteBlobLSBLong(image,(unsigned int) image->rows);     
    (void) WriteBlobLSBLong(image,(unsigned int) image->columns);  
    if (is_gray == MagickFalse)
      {
        (void) WriteBlobLSBLong(image,3);  
        (void) WriteBlobLSBLong(image,0);
      }
    (void) WriteBlobLSBShort(image,1);   
    (void) WriteBlobLSBShort(image,1);   
    (void) WriteBlobLSBLong(image,'M');  
    (void) WriteBlobLSBLong(image,0x2);  
    (void) WriteBlobLSBLong(image,(unsigned int) data_size);  

     
    quantum_info=AcquireQuantumInfo(image_info,image);
    if (quantum_info == (QuantumInfo *) NULL)
      ThrowWriterException(ResourceLimitError,"MemoryAllocationFailed");
    pixels=(unsigned char *) GetQuantumPixels(quantum_info);
    do
    {
      const Quantum
        *p;

      ssize_t
        y;

      for (y=0; y < (ssize_t) image->columns; y++)
      {
        size_t
          length;

        p=GetVirtualPixels(image,y,0,1,image->rows,exception);
        if (p == (const Quantum *) NULL)
          break;
        length=ExportQuantumPixels(image,(CacheView *) NULL,quantum_info,
          z2qtype[z],pixels,exception);
        if (length != image->columns)
          break;
        if (WriteBlob(image,image->rows,pixels) != (ssize_t) image->rows)
          break;
      }
      if (y < (ssize_t) image->columns)
        break;
      if (SyncAuthenticPixels(image,exception) == MagickFalse)
        break;
    } while (z-- >= 2);
    while (padding-- > 0)
      (void) WriteBlobByte(image,0);
    quantum_info=DestroyQuantumInfo(quantum_info);
    if (GetNextImageInList(image) == (Image *) NULL)
      break;
    image=SyncNextImageInList(image);
    status=SetImageProgress(image,SaveImagesTag,scene++,imageListLength);
    if (status == MagickFalse)
      break;
  } while (image_info->adjoin != MagickFalse);
  (void) CloseBlob(image);
  return(status);
}