void RenderFrameHostImpl::OnDidBlockFramebust(const GURL& url) {
  delegate_->OnDidBlockFramebust(url);
}
