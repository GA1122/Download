void RenderViewImpl::SetActiveForWidget(bool active) {
  if (webview())
    webview()->SetIsActive(active);
}
