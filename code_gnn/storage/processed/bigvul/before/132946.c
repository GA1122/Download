gfx::GLSurfaceHandle RenderWidgetHostViewAura::GetCompositingSurface() {
  return ImageTransportFactory::GetInstance()->GetSharedSurfaceHandle();
}
