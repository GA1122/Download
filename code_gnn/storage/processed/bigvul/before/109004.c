void RenderViewImpl::OnZoom(content::PageZoom zoom) {
  if (!webview())   
    return;

  webview()->hidePopups();

  double old_zoom_level = webview()->zoomLevel();
  double zoom_level;
  if (zoom == content::PAGE_ZOOM_RESET) {
    zoom_level = 0;
  } else if (static_cast<int>(old_zoom_level) == old_zoom_level) {
    zoom_level = old_zoom_level + zoom;
  } else {
    if ((old_zoom_level > 1 && zoom > 0) ||
        (old_zoom_level < 1 && zoom < 0)) {
      zoom_level = static_cast<int>(old_zoom_level + zoom);
    } else {
      zoom_level = static_cast<int>(old_zoom_level);
    }
  }
  webview()->setZoomLevel(false, zoom_level);
  zoomLevelChanged();
}
