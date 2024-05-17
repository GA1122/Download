void RenderFrameImpl::OnDeleteFrame() {
  in_browser_initiated_detach_ = true;

  frame_->Detach();
}
