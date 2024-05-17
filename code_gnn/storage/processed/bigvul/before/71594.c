static MagickBooleanType WriteMATImage(const ImageInfo *image_info,Image *image)
{
  ExceptionInfo
    *exception;

  ssize_t y;
  unsigned z;
  const PixelPacket *p;

  unsigned int status;
  int logging;
  size_t DataSize;
  char padding;
  char MATLAB_HDR[0x80];
  time_t current_time;
  struct tm local_time;
  unsigned char *pixels;
  int is_gray;

  MagickOffsetType
    scene;

  QuantumInfo
    *quantum_info;

   
  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickSignature);
  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  logging=LogMagickEvent(CoderEvent,GetMagickModule(),"enter MAT");
  (void) logging;
  status=OpenBlob(image_info,image,WriteBinaryBlobMode,&image->exception);
  if (status == MagickFalse)
    return(MagickFalse);
  image->depth=8;

  current_time=time((time_t *) NULL);
#if defined(MAGICKCORE_HAVE_LOCALTIME_R)
  (void) localtime_r(&current_time,&local_time);
#else
  (void) memcpy(&local_time,localtime(&current_time),sizeof(local_time));
#endif
  (void) memset(MATLAB_HDR,' ',MagickMin(sizeof(MATLAB_HDR),124));
  FormatLocaleString(MATLAB_HDR,sizeof(MATLAB_HDR),
    "MATLAB 5.0 MAT-file, Platform: %s, Created on: %s %s %2d %2d:%2d:%2d %d",
    OsDesc,DayOfWTab[local_time.tm_wday],MonthsTab[local_time.tm_mon],
    local_time.tm_mday,local_time.tm_hour,local_time.tm_min,
    local_time.tm_sec,local_time.tm_year+1900);
  MATLAB_HDR[0x7C]=0;
  MATLAB_HDR[0x7D]=1;
  MATLAB_HDR[0x7E]='I';
  MATLAB_HDR[0x7F]='M';
  (void) WriteBlob(image,sizeof(MATLAB_HDR),(unsigned char *) MATLAB_HDR);
  scene=0;
  do
  {
    (void) TransformImageColorspace(image,sRGBColorspace);
    is_gray = IsGrayImage(image,&image->exception);
    z = is_gray ? 0 : 3;

     
    DataSize = image->rows   * image->columns  ;
    if(!is_gray) DataSize *= 3  ;
    padding=((unsigned char)(DataSize-1) & 0x7) ^ 0x7;

    (void) WriteBlobLSBLong(image, miMATRIX);
    (void) WriteBlobLSBLong(image, (unsigned int) DataSize+padding+(is_gray ? 48 : 56));
    (void) WriteBlobLSBLong(image, 0x6);  
    (void) WriteBlobLSBLong(image, 0x8);  
    (void) WriteBlobLSBLong(image, 0x6);  
    (void) WriteBlobLSBLong(image, 0);
    (void) WriteBlobLSBLong(image, 0x5);  
    (void) WriteBlobLSBLong(image, is_gray ? 0x8 : 0xC);  
    (void) WriteBlobLSBLong(image, (unsigned int) image->rows);     
    (void) WriteBlobLSBLong(image, (unsigned int) image->columns);  
    if(!is_gray)
    {
      (void) WriteBlobLSBLong(image, 3);  
      (void) WriteBlobLSBLong(image, 0);
    }
    (void) WriteBlobLSBShort(image, 1);   
    (void) WriteBlobLSBShort(image, 1);   
    (void) WriteBlobLSBLong(image, 'M');  
    (void) WriteBlobLSBLong(image, 0x2);  
    (void) WriteBlobLSBLong(image, (unsigned int) DataSize);  

     
    exception=(&image->exception);
    quantum_info=AcquireQuantumInfo(image_info,image);
    if (quantum_info == (QuantumInfo *) NULL)
      ThrowWriterException(ResourceLimitError,"MemoryAllocationFailed");
    pixels=GetQuantumPixels(quantum_info);
    do
    {
      for (y=0; y < (ssize_t)image->columns; y++)
      {
        p=GetVirtualPixels(image,y,0,1,image->rows,&image->exception);
        if (p == (const PixelPacket *) NULL)
          break;
        (void) ExportQuantumPixels(image,(const CacheView *) NULL,quantum_info,
          z2qtype[z],pixels,exception);
        (void) WriteBlob(image,image->rows,pixels);
      }
      if (!SyncAuthenticPixels(image,exception))
        break;
    } while(z-- >= 2);
    while(padding-->0) (void) WriteBlobByte(image,0);
    quantum_info=DestroyQuantumInfo(quantum_info);
    if (GetNextImageInList(image) == (Image *) NULL)
      break;
    image=SyncNextImageInList(image);
    status=SetImageProgress(image,SaveImagesTag,scene++,
      GetImageListLength(image));
    if (status == MagickFalse)
      break;
  } while (image_info->adjoin != MagickFalse);
  (void) CloseBlob(image);
  return(MagickTrue);
}
