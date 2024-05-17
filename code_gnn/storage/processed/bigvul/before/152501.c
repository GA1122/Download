void RenderFrameImpl::ResumeBlockedRequests() {
  frame_request_blocker_->Resume();
}
