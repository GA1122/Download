static void ipa_device_end(wmfAPI * API)
{
  wmf_magick_t
    *ddata = WMF_MAGICK_GetData(API);

   
  if (ddata->clipping)
    (void) PopDrawingWand(WmfDrawingWand);
  ddata->clipping = MagickFalse;

   
  (void) PopDrawingWand(WmfDrawingWand);
}
