void RenderWidgetHostViewAndroid::SelectionBoundsChanged(
    const gfx::Rect& start_rect,
    WebKit::WebTextDirection start_direction,
    const gfx::Rect& end_rect,
    WebKit::WebTextDirection end_direction) {
  if (content_view_core_) {
    content_view_core_->OnSelectionBoundsChanged(
        start_rect,
        ConvertTextDirection(start_direction),
        end_rect,
        ConvertTextDirection(end_direction));
  }
}
