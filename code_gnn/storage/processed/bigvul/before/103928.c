void RenderView::zoomLimitsChanged(double minimum_level, double maximum_level) {
  bool remember = !webview()->mainFrame()->document().isPluginDocument();

  int minimum_percent = static_cast<int>(
      WebView::zoomLevelToZoomFactor(minimum_level) * 100);
  int maximum_percent = static_cast<int>(
      WebView::zoomLevelToZoomFactor(maximum_level) * 100);

  Send(new ViewHostMsg_UpdateZoomLimits(
      routing_id_, minimum_percent, maximum_percent, remember));
}
