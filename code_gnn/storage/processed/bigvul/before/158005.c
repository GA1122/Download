void RenderViewImpl::UpdateZoomLevel(double zoom_level) {
  webview()->HidePopups();
  SetZoomLevel(zoom_level);
}
