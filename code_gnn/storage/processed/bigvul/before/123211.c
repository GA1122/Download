void RenderWidgetHostViewAura::OnBoundsChanged(const gfx::Rect& old_bounds,
                                               const gfx::Rect& new_bounds) {
  if (is_fullscreen_)
    SetSize(new_bounds.size());
}
