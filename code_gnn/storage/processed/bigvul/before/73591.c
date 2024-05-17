static MagickBooleanType load_hierarchy(Image *image,XCFDocInfo *inDocInfo,
   XCFLayerInfo *inLayer, ExceptionInfo *exception)
{
  MagickOffsetType
    saved_pos,
    offset,
    junk;

  size_t
    width,
    height,
    bytes_per_pixel;

  width=ReadBlobMSBLong(image);
  (void) width;
  height=ReadBlobMSBLong(image);
  (void) height;
  bytes_per_pixel=inDocInfo->bytes_per_pixel=ReadBlobMSBLong(image);
  (void) bytes_per_pixel;

   
  offset=(MagickOffsetType) ReadBlobMSBLong(image);   

   
  do
  {
    junk=(MagickOffsetType) ReadBlobMSBLong(image);
  }
  while (junk != 0);

   
  saved_pos=TellBlob(image);

   
  offset=SeekBlob(image, offset, SEEK_SET);

   
  if (load_level (image, inDocInfo, inLayer, exception) == 0)
    return(MagickFalse);
   
  offset=SeekBlob(image, saved_pos, SEEK_SET);
  return(MagickTrue);
}
