bool VaapiVideoDecodeAccelerator::VaapiVP8Accelerator::OutputPicture(
    const scoped_refptr<VP8Picture>& pic) {
  scoped_refptr<VaapiDecodeSurface> dec_surface =
      VP8PictureToVaapiDecodeSurface(pic);

  vaapi_dec_->SurfaceReady(dec_surface);
  return true;
}
