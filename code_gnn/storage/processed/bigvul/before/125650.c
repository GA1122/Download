void RenderViewHostImpl::OnDidDisownOpener() {
  delegate_->DidDisownOpener(this);
}
