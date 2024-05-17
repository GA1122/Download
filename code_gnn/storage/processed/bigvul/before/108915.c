void RenderViewImpl::OnClearFocusedNode() {
  if (webview())
    webview()->clearFocusedNode();
}
