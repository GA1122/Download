void RenderViewImpl::SetPageFrozen(bool frozen) {
  if (webview())
    webview()->SetPageFrozen(frozen);
}
