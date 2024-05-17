void RenderView::zoomLevelChanged() {
  bool remember = !webview()->mainFrame()->document().isPluginDocument();

  Send(new ViewHostMsg_DidZoomURL(
      routing_id_, webview()->zoomLevel(), remember,
      GURL(webview()->mainFrame()->document().url())));
}
