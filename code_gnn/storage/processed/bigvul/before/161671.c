bool VaapiVideoDecodeAccelerator::DecodeSurface(
    const scoped_refptr<VaapiDecodeSurface>& dec_surface) {
  const bool result = vaapi_wrapper_->ExecuteAndDestroyPendingBuffers(
      dec_surface->va_surface()->id());
  if (!result)
    VLOGF(1) << "Failed decoding picture";
  return result;
}
