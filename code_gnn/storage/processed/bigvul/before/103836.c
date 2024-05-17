void RenderView::OnSetZoomLevel(double zoom_level) {
  if (webview()->mainFrame()->document().isPluginDocument())
    return;

  webview()->hidePopups();
  webview()->setZoomLevel(false, zoom_level);
  zoomLevelChanged();
}
