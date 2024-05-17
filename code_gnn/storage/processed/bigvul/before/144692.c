void WebContentsImpl::SwappedOut(RenderFrameHost* rfh) {
  if (delegate_ && rfh->GetRenderViewHost() == GetRenderViewHost())
    delegate_->SwappedOut(this);
}
