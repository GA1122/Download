GLSurfaceOzoneEGL::GLSurfaceOzoneEGL(
    std::unique_ptr<ui::SurfaceOzoneEGL> ozone_surface,
    AcceleratedWidget widget)
    : NativeViewGLSurfaceEGL(ozone_surface->GetNativeWindow()),
      ozone_surface_(std::move(ozone_surface)),
      widget_(widget) {}
