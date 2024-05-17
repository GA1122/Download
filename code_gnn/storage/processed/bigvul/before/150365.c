gfx::Rect ClientControlledShellSurface::GetShadowBounds() const {
  gfx::Rect shadow_bounds = ShellSurfaceBase::GetShadowBounds();
  const ash::NonClientFrameViewAsh* frame_view = GetFrameView();
  if (frame_view->GetVisible()) {
    shadow_bounds.set_size(
        frame_view->GetWindowBoundsForClientBounds(shadow_bounds).size());
  }

  return shadow_bounds;
}
