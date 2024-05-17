gfx::GLSurfaceHandle RenderWidgetHostImpl::GetCompositingSurface() {
  if (view_)
    return view_->GetCompositingSurface();
  return gfx::GLSurfaceHandle();
}
