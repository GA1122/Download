gfx::Size RenderWidgetHostViewAura::GetVisibleViewportSize() const {
  gfx::Rect requested_rect(GetRequestedRendererSize());
  requested_rect.Inset(insets_);
  return requested_rect.size();
}
