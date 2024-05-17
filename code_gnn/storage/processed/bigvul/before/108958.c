void RenderViewImpl::OnReloadFrame() {
  if (webview() && webview()->focusedFrame()) {
    webview()->focusedFrame()->reload(false);
  }
}
