void RenderWidgetHostImpl::SetView(RenderWidgetHostView* view) {
  view_ = RenderWidgetHostViewPort::FromRWHV(view);

  if (!view_) {
    GpuSurfaceTracker::Get()->SetSurfaceHandle(
        surface_id_, gfx::GLSurfaceHandle());
  }
}
