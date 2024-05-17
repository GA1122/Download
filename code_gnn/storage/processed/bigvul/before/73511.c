MagickExport VirtualPixelMethod SetPixelCacheVirtualMethod(const Image *image,
  const VirtualPixelMethod virtual_pixel_method)
{
  CacheInfo
    *magick_restrict cache_info;

  VirtualPixelMethod
    method;

  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  assert(image->cache != (Cache) NULL);
  cache_info=(CacheInfo *) image->cache;
  assert(cache_info->signature == MagickSignature);
  method=cache_info->virtual_pixel_method;
  cache_info->virtual_pixel_method=virtual_pixel_method;
  if ((image->columns != 0) && (image->rows != 0))
    switch (virtual_pixel_method)
    {
      case BackgroundVirtualPixelMethod:
      {
        if ((image->background_color.opacity != OpaqueOpacity) &&
            (image->matte == MagickFalse))
          (void) SetCacheAlphaChannel((Image *) image,OpaqueOpacity);
        if ((IsPixelGray(&image->background_color) == MagickFalse) &&
            (IsGrayColorspace(image->colorspace) != MagickFalse))
          (void) SetImageColorspace((Image *) image,sRGBColorspace);
        break;
      }
      case TransparentVirtualPixelMethod:
      {
        if (image->matte == MagickFalse)
          (void) SetCacheAlphaChannel((Image *) image,OpaqueOpacity);
        break;
      }
      default:
        break;
    }
  return(method);
}
