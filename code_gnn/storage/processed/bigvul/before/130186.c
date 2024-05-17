void RenderFrameHostImpl::OnDidStartLoading() {
  delegate_->DidStartLoading(this);
}
