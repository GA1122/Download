void RenderFrameHostImpl::OnDidStopLoading() {
  delegate_->DidStopLoading(this);
}
