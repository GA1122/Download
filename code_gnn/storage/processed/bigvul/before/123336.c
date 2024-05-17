void RenderWidgetHostViewGtk::SelectionBoundsChanged(
    const gfx::Rect& start_rect,
    WebKit::WebTextDirection start_direction,
    const gfx::Rect& end_rect,
    WebKit::WebTextDirection end_direction) {
  im_context_->UpdateCaretBounds(gfx::UnionRects(start_rect, end_rect));
}
