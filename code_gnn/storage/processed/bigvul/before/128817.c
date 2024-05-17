bool GLSurfaceOzoneEGL::Resize(const gfx::Size& size,
                               float scale_factor,
                               bool has_alpha) {
  if (!ozone_surface_->ResizeNativeWindow(size)) {
    if (!ReinitializeNativeSurface() ||
        !ozone_surface_->ResizeNativeWindow(size))
      return false;
  }

  return NativeViewGLSurfaceEGL::Resize(size, scale_factor, has_alpha);
}
