void RenderFrameHostImpl::CancelBlockedRequestsForFrame() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  NotifyForEachFrameFromUI(
      this,
      base::Bind(&ResourceDispatcherHostImpl::CancelBlockedRequestsForRoute));
}
