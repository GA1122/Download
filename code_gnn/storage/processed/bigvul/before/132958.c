gfx::Rect RenderWidgetHostViewAura::GetViewBounds() const {
  gfx::Rect bounds(window_->GetBoundsInScreen());
  if (resize_lock_.get())
    return gfx::Rect(bounds.origin(), resize_lock_->expected_size());
  else
    return bounds;
}
