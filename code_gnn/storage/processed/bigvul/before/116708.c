void RenderViewImpl::zoomLevelChanged() {
  bool remember = !webview()->mainFrame()->document().isPluginDocument();
  float zoom_level = webview()->zoomLevel();
  Send(new ViewHostMsg_DidZoomURL(
      routing_id_, zoom_level, remember,
      GURL(webview()->mainFrame()->document().url())));
}
