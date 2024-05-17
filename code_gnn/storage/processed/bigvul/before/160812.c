void RenderViewImpl::OnSetFocus(bool enable) {
  CHECK(webview()->MainFrame()->IsWebLocalFrame());
  SetFocus(enable);
}
