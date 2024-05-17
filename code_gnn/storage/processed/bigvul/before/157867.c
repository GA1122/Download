void WebContentsImpl::SetVisibilityForChildViews(bool visible) {
  GetMainFrame()->SetVisibilityForChildViews(visible);
}
