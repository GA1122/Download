void RenderWidgetHostViewAura::EnsureCaretNotInRect(const gfx::Rect& rect) {
  gfx::Rect rect_in_local_space = ConvertRectFromScreen(rect);
  gfx::Rect hiding_area_in_this_window =
      gfx::IntersectRects(rect_in_local_space, window_->bounds());

  if (hiding_area_in_this_window.IsEmpty())
    return;

  host_->ScrollFocusedEditableNodeIntoRect(
      gfx::SubtractRects(window_->bounds(), hiding_area_in_this_window));
}
