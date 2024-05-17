void RenderFrameHostImpl::BlockRequestsForFrame() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  NotifyForEachFrameFromUI(
      this,
      base::BindRepeating(&ResourceDispatcherHostImpl::BlockRequestsForRoute));
}
