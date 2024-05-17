gfx::GLSurfaceHandle RenderWidgetHostViewAndroid::GetCompositingSurface() {
  if (CompositorImpl::IsInitialized()) {
    if (surface_texture_transport_.get()) {
      return surface_texture_transport_->GetCompositingSurface(
          host_->surface_id());
    } else {
      if (shared_surface_.is_null()) {
        shared_surface_ =
            ImageTransportFactoryAndroid::GetInstance()->
            CreateSharedSurfaceHandle();
      }
      return shared_surface_;
    }
  }

  return gfx::GLSurfaceHandle(gfx::kNullPluginWindow, true);
}
