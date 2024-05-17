GLSurfaceOzoneSurfaceless::GLSurfaceOzoneSurfaceless(
    std::unique_ptr<ui::SurfaceOzoneEGL> ozone_surface,
    AcceleratedWidget widget)
    : SurfacelessEGL(gfx::Size()),
      ozone_surface_(std::move(ozone_surface)),
      widget_(widget),
      has_implicit_external_sync_(
          HasEGLExtension("EGL_ARM_implicit_external_sync")),
      last_swap_buffers_result_(true),
      swap_buffers_pending_(false),
      weak_factory_(this) {
  unsubmitted_frames_.push_back(new PendingFrame());
}
