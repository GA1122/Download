void RenderViewImpl::DidReceiveSetFocusEventForWidget() {
  CHECK(webview()->MainFrame()->IsWebLocalFrame());
}
