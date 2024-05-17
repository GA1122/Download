void RenderViewImpl::OnMoveOrResizeStarted() {
  if (webview())
    webview()->HidePopups();
}
