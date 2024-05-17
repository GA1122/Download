void RenderFrameHostImpl::OnDocumentOnLoadCompleted() {
  delegate_->DocumentOnLoadCompleted(this);
}
