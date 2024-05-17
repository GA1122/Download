scoped_refptr<GLSurface> CreateViewGLSurfaceOzoneSurfacelessSurfaceImpl(
    gfx::AcceleratedWidget window) {
  std::unique_ptr<ui::SurfaceOzoneEGL> surface_ozone =
      ui::OzonePlatform::GetInstance()
          ->GetSurfaceFactoryOzone()
          ->CreateSurfacelessEGLSurfaceForWidget(window);
  if (!surface_ozone)
    return nullptr;
  scoped_refptr<GLSurface> surface = new GLSurfaceOzoneSurfacelessSurfaceImpl(
      std::move(surface_ozone), window);
  if (!surface->Initialize())
    return nullptr;
  return surface;
}
