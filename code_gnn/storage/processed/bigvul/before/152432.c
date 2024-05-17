void RenderFrameImpl::OnEnableViewSourceMode() {
  DCHECK(frame_);
  DCHECK(!frame_->Parent());
  frame_->EnableViewSourceMode(true);
}
