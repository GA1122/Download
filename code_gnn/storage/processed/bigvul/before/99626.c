bool VaapiVideoDecodeAccelerator::DecodeSurface(
    const scoped_refptr<VaapiDecodeSurface>& dec_surface) {
  if (!vaapi_wrapper_->ExecuteAndDestroyPendingBuffers(
          dec_surface->va_surface()->id())) {
    DVLOG(1) << "Failed decoding picture";
    return false;
  }

  return true;
}
