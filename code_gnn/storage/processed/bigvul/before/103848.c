void RenderView::OnZoom(PageZoom::Function function) {
  if (!webview())   
    return;

  webview()->hidePopups();

  double old_zoom_level = webview()->zoomLevel();
  double zoom_level;
  if (function == PageZoom::RESET) {
    zoom_level = 0;
  } else if (static_cast<int>(old_zoom_level) == old_zoom_level) {
    zoom_level = old_zoom_level + function;
  } else {
    if ((old_zoom_level > 1 && function > 0) ||
        (old_zoom_level < 1 && function < 0)) {
      zoom_level = static_cast<int>(old_zoom_level + function);
    } else {
      zoom_level = static_cast<int>(old_zoom_level);
    }
  }

  webview()->setZoomLevel(false, zoom_level);
  zoomLevelChanged();
}
