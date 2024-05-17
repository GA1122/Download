MagickExport void GetPixelCacheTileSize(const Image *image,size_t *width,
  size_t *height)
{
  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  *width=2048UL/sizeof(PixelPacket);
  if (GetImagePixelCacheType(image) == DiskCache)
    *width=8192UL/sizeof(PixelPacket);
  *height=(*width);
}
