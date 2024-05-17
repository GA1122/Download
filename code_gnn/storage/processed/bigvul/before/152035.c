void RenderFrameHostImpl::OnFrameDidCallFocus() {
  delegate_->DidCallFocus();
}
