bool VaapiVideoDecodeAccelerator::VaapiH264Accelerator::OutputPicture(
    const scoped_refptr<H264Picture>& pic) {
  scoped_refptr<VaapiDecodeSurface> dec_surface =
      H264PictureToVaapiDecodeSurface(pic);

  vaapi_dec_->SurfaceReady(dec_surface);

  return true;
}
