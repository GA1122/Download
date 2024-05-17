void RenderViewImpl::OnSetActive(bool active) {
  if (webview())
    webview()->SetIsActive(active);
}
