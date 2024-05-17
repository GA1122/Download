void RenderViewImpl::Close() {
  WebView* doomed = webview();
  RenderWidget::Close();
  g_view_map.Get().erase(doomed);
}
