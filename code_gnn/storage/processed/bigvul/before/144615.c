void WebContentsImpl::OnRenderFrameProxyVisibilityChanged(bool visible) {
  if (visible)
    WasShown();
  else
    WasHidden();
}
