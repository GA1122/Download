void RenderViewHostImpl::OnDidStartLoading() {
  delegate_->DidStartLoading(this);
}
