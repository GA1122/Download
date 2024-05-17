void RenderWidgetHostViewAura::OnWindowSurfaceChanged(
    const cc::SurfaceInfo& surface_info) {
  if (!is_guest_view_hack_)
    return;
  host_->GetView()->OnSurfaceChanged(surface_info);
}
