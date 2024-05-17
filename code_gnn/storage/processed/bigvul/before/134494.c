void WebContentsViewAura::GetContainerBounds(gfx::Rect *out) const {
  *out = window_->GetBoundsInScreen();
}
