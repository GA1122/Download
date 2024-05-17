gfx::GLSurfaceHandle RenderWidgetHostViewAura::GetCompositingSurface() {
  if (shared_surface_handle_.is_null()) {
    ImageTransportFactory* factory = ImageTransportFactory::GetInstance();
    shared_surface_handle_ = factory->CreateSharedSurfaceHandle();
    factory->AddObserver(this);
  }
  return shared_surface_handle_;
}
