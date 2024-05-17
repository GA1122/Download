bool GLSurfaceOzoneSurfaceless::Initialize(gfx::GLSurface::Format format) {
  if (!SurfacelessEGL::Initialize(format))
    return false;
  vsync_provider_ = ozone_surface_->CreateVSyncProvider();
  if (!vsync_provider_)
    return false;
  return true;
}
