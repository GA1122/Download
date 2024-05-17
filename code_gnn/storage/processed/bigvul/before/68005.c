static MagickBooleanType load_level(Image *image,XCFDocInfo *inDocInfo,
  XCFLayerInfo *inLayerInfo)
{
  ExceptionInfo
    *exception;

  int
    destLeft = 0,
    destTop = 0;

  Image*
    tile_image;

  MagickBooleanType
    status;

  MagickOffsetType
    saved_pos,
    offset,
    offset2;

  register ssize_t
    i;

  size_t
    width,
    height,
    ntiles,
    ntile_rows,
    ntile_cols,
    tile_image_width,
    tile_image_height;

   
  exception=inDocInfo->exception;
  width=ReadBlobMSBLong(image);
  height=ReadBlobMSBLong(image);

   
  offset=(MagickOffsetType) ReadBlobMSBLong(image);
  if (offset == 0)
    return(MagickTrue);
   
  ntile_rows=(height+TILE_HEIGHT-1)/TILE_HEIGHT;
  ntile_cols=(width+TILE_WIDTH-1)/TILE_WIDTH;
  ntiles=ntile_rows*ntile_cols;
  for (i = 0; i < (ssize_t) ntiles; i++)
  {
    status=MagickFalse;
    if (offset == 0)
      ThrowBinaryException(CorruptImageError,"NotEnoughTiles",image->filename);
     
    saved_pos=TellBlob(image);
     
    offset2=(MagickOffsetType)ReadBlobMSBLong(image);
     
    if (offset2 == 0)
      offset2=(MagickOffsetType) (offset + TILE_WIDTH * TILE_WIDTH * 4* 1.5);
     
    if (SeekBlob(image, offset, SEEK_SET) != offset)
      ThrowBinaryException(CorruptImageError,"InsufficientImageDataInFile",
        image->filename);

       
      tile_image_width=(size_t) (destLeft == (int) ntile_cols-1 ?
        (int) width % TILE_WIDTH : TILE_WIDTH);
      if (tile_image_width == 0)
        tile_image_width=TILE_WIDTH;
      tile_image_height = (size_t) (destTop == (int) ntile_rows-1 ?
        (int) height % TILE_HEIGHT : TILE_HEIGHT);
      if (tile_image_height == 0)
        tile_image_height=TILE_HEIGHT;
      tile_image=CloneImage(inLayerInfo->image,tile_image_width,
        tile_image_height,MagickTrue,exception);

       
      switch (inDocInfo->compression)
      {
        case COMPRESS_NONE:
          if (load_tile(image,tile_image,inDocInfo,inLayerInfo,(size_t) (offset2-offset)) == 0)
            status=MagickTrue;
          break;
        case COMPRESS_RLE:
          if (load_tile_rle (image,tile_image,inDocInfo,inLayerInfo,
              (int) (offset2-offset)) == 0)
            status=MagickTrue;
          break;
        case COMPRESS_ZLIB:
          ThrowBinaryException(CoderError,"ZipCompressNotSupported",
            image->filename)
        case COMPRESS_FRACTAL:
          ThrowBinaryException(CoderError,"FractalCompressNotSupported",
            image->filename)
      }

       
      (void) CompositeImage(inLayerInfo->image,CopyCompositeOp,tile_image,
        destLeft * TILE_WIDTH,destTop*TILE_HEIGHT);
      tile_image=DestroyImage(tile_image);

       
      destLeft++;
      if (destLeft >= (int) ntile_cols)
        {
          destLeft = 0;
          destTop++;
        }
      if (status != MagickFalse)
        return(MagickFalse);
       
      offset=SeekBlob(image, saved_pos, SEEK_SET);
       
      offset=(MagickOffsetType) ReadBlobMSBLong(image);
    }
  if (offset != 0)
    ThrowBinaryException(CorruptImageError,"CorruptImage",image->filename)
  return(MagickTrue);
}
