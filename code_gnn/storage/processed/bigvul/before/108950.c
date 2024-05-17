void RenderViewImpl::OnOrientationChangeEvent(int orientation) {
  webview()->mainFrame()->sendOrientationChangeEvent(orientation);
}
