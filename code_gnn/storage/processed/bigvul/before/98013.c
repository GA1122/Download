void RenderView::OnZoom(PageZoom::Function function) {
  if (!webview())   
    return;

  webview()->hidePopups();

  int zoom_level = webview()->zoomLevel();
  int new_zoom_level = webview()->setZoomLevel(false,
      (function == PageZoom::RESET) ? 0 : (zoom_level + function));

  Send(new ViewHostMsg_DidZoomURL(
      GURL(webview()->mainFrame()->url()), new_zoom_level));
}
