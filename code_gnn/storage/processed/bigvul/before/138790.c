void RenderFrameHostImpl::ResumeBlockedRequestsForFrame() {
  NotifyForEachFrameFromUI(
      this,
      base::Bind(&ResourceDispatcherHostImpl::ResumeBlockedRequestsForRoute));
}
