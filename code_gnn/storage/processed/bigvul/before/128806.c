bool GLSurfaceOzoneEGL::Initialize(gfx::GLSurface::Format format) {
  format_ = format;
  return Initialize(ozone_surface_->CreateVSyncProvider());
}
