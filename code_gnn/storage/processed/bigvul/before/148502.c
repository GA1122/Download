void WebContentsImpl::OnRenderFrameProxyVisibilityChanged(bool visible) {
  if (visible && !GetOuterWebContents()->IsHidden())
    WasShown();
  else if (!visible)
    WasHidden();
}
