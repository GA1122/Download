void RenderFrameHostImpl::DidReceiveFirstUserActivation() {
  delegate_->DidReceiveFirstUserActivation(this);
}
