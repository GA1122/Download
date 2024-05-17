void RenderViewHostImpl::OnSelectionBoundsChanged(
    const gfx::Rect& start_rect,
    WebKit::WebTextDirection start_direction,
    const gfx::Rect& end_rect,
    WebKit::WebTextDirection end_direction) {
  if (view_) {
    view_->SelectionBoundsChanged(start_rect, start_direction,
                                  end_rect, end_direction);
  }
}
