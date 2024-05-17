gfx::Point BrowserView::OffsetPointForToolbarBackgroundImage(
    const gfx::Point& point) const {
  gfx::Point window_point(point + GetMirroredPosition().OffsetFromOrigin());
  window_point.Offset(frame_->GetThemeBackgroundXInset(),
                      -frame_->GetTabStripInsets(false).top);
  return window_point;
}