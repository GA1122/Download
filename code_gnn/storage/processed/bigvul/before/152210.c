void RenderFrameImpl::CancelBlockedRequests() {
  frame_request_blocker_->Cancel();
}
