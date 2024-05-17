GLSurfaceOzoneSurfacelessSurfaceImpl::GLSurfaceOzoneSurfacelessSurfaceImpl(
    std::unique_ptr<ui::SurfaceOzoneEGL> ozone_surface,
    AcceleratedWidget widget)
    : GLSurfaceOzoneSurfaceless(std::move(ozone_surface), widget),
      context_(nullptr),
      fbo_(0),
      current_surface_(0) {
  for (auto& texture : textures_)
    texture = 0;
}
