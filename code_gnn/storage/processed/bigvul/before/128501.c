void ShellSurface::OnWindowBoundsChanged(aura::Window* window,
                                         const gfx::Rect& old_bounds,
                                         const gfx::Rect& new_bounds) {
  if (!widget_ || !surface_ || ignore_window_bounds_changes_)
    return;

  if (window == widget_->GetNativeWindow()) {
    if (new_bounds.size() == old_bounds.size())
      return;

    gfx::Vector2d origin_offset = new_bounds.origin() - old_bounds.origin();
    pending_origin_config_offset_ += origin_offset;
    origin_ -= origin_offset;

    surface_->window()->SetBounds(
        gfx::Rect(GetSurfaceOrigin(), surface_->window()->layer()->size()));

    Configure();
  }
}
