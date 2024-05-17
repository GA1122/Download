scoped_refptr<GLSurface> GLSurface::CreateSurfacelessViewGLSurface(
    gfx::AcceleratedWidget window) {
  if (GetGLImplementation() == kGLImplementationEGLGLES2 &&
      window != kNullAcceleratedWidget &&
      GLSurfaceEGL::IsEGLSurfacelessContextSupported()) {
    std::unique_ptr<ui::SurfaceOzoneEGL> surface_ozone =
        ui::OzonePlatform::GetInstance()
            ->GetSurfaceFactoryOzone()
            ->CreateSurfacelessEGLSurfaceForWidget(window);
    if (!surface_ozone)
      return nullptr;
    scoped_refptr<GLSurface> surface;
    surface = new GLSurfaceOzoneSurfaceless(std::move(surface_ozone), window);
    if (surface->Initialize())
      return surface;
  }

  return nullptr;
}
