void RenderFrameImpl::BlockRequests() {
  frame_request_blocker_->Block();
}
