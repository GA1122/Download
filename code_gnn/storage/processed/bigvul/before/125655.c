void RenderViewHostImpl::OnDidStopLoading() {
  delegate_->DidStopLoading(this);
}
