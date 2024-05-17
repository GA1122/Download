void RenderViewImpl::OnSetZoomLevel(double zoom_level) {
  webview()->hidePopups();
  webview()->setZoomLevel(false, zoom_level);
  zoomLevelChanged();
}
