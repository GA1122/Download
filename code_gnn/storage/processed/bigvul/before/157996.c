void RenderViewImpl::SetZoomLevel(double zoom_level) {
  page_zoom_level_ = zoom_level;

  webview()->SetZoomLevel(zoom_level);
  for (auto& observer : observers_)
    observer.OnZoomLevelChanged();
}
