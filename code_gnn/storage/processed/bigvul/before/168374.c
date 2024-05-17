void BrowserView::ToolbarSizeChanged(bool is_animating) {
  if (is_animating)
    contents_web_view_->SetFastResize(true);
  UpdateUIForContents(GetActiveWebContents());
  if (is_animating)
    contents_web_view_->SetFastResize(false);

  if (!is_animating) {
    contents_web_view_->InvalidateLayout();
    contents_container_->Layout();
  }
}
