scoped_refptr<GLSurface> CreateViewGLSurfaceOzone(
    gfx::AcceleratedWidget window) {
  std::unique_ptr<ui::SurfaceOzoneEGL> surface_ozone =
      ui::OzonePlatform::GetInstance()
          ->GetSurfaceFactoryOzone()
          ->CreateEGLSurfaceForWidget(window);
  if (!surface_ozone)
    return nullptr;
  scoped_refptr<GLSurface> surface =
      new GLSurfaceOzoneEGL(std::move(surface_ozone), window);
  if (!surface->Initialize())
    return nullptr;
  return surface;
}
