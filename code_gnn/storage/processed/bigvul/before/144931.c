void RenderWidgetHostViewAura::InternalSetBounds(const gfx::Rect& rect) {
  SnapToPhysicalPixelBoundary();
  if (!in_bounds_changed_)
    window_->SetBounds(rect);
  host_->WasResized();
  if (delegated_frame_host_)
    delegated_frame_host_->WasResized();
#if defined(OS_WIN)
  UpdateLegacyWin();

  if (mouse_locked_)
    UpdateMouseLockRegion();
#endif
}
