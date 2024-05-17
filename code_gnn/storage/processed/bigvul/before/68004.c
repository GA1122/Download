static MagickBooleanType load_hierarchy(Image *image,XCFDocInfo *inDocInfo,
   XCFLayerInfo *inLayer)
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

   
  if (SeekBlob(image, offset, SEEK_SET) != offset)
    ThrowBinaryException(CorruptImageError,"InsufficientImageDataInFile",
      image->filename);

   
  if (load_level (image, inDocInfo, inLayer) == 0)
    return(MagickFalse);
   
  offset=SeekBlob(image, saved_pos, SEEK_SET);
  return(MagickTrue);
}
