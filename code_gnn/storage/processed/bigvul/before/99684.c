scoped_refptr<VASurface> VaapiWrapper::CreateUnownedSurface(
    unsigned int va_format,
    const gfx::Size& size,
    const std::vector<VASurfaceAttrib>& va_attribs) {
  base::AutoLock auto_lock(*va_lock_);

  std::vector<VASurfaceAttrib> attribs(va_attribs);
  VASurfaceID va_surface_id;
  VAStatus va_res =
      vaCreateSurfaces(va_display_, va_format, size.width(), size.height(),
                       &va_surface_id, 1, &attribs[0], attribs.size());

  scoped_refptr<VASurface> va_surface;
  VA_SUCCESS_OR_RETURN(va_res, "Failed to create unowned VASurface",
                       va_surface);

  va_surface = new VASurface(
      va_surface_id, size,
      base::Bind(&VaapiWrapper::DestroyUnownedSurface, base::Unretained(this)));

  return va_surface;
}
