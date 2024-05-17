bool GLSurfaceOzoneSurfaceless::Resize(const gfx::Size& size,
                                       float scale_factor,
                                       bool has_alpha) {
  if (!ozone_surface_->ResizeNativeWindow(size))
    return false;

  return SurfacelessEGL::Resize(size, scale_factor, has_alpha);
}
