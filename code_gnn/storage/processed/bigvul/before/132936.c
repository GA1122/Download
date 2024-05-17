void RenderWidgetHostViewAura::EnsureCaretInRect(const gfx::Rect& rect) {
  gfx::Rect intersected_rect(
      gfx::IntersectRects(rect, window_->GetBoundsInScreen()));

  if (intersected_rect.IsEmpty())
    return;

  host_->ScrollFocusedEditableNodeIntoRect(
      ConvertRectFromScreen(intersected_rect));
}
