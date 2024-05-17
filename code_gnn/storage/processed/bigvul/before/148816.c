void InterstitialPageImpl::TakeActionOnResourceDispatcher(
    ResourceRequestAction action) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  if (action == CANCEL || action == RESUME) {
    if (resource_dispatcher_host_notified_)
      return;
    resource_dispatcher_host_notified_ = true;
  }

  RenderViewHostImpl* rvh = RenderViewHostImpl::FromID(original_child_id_,
                                                       original_rvh_id_);
  if (!rvh)
    return;

  RenderFrameHostImpl* rfh =
      static_cast<RenderFrameHostImpl*>(rvh->GetMainFrame());
  if (!rfh)
    return;

  switch (action) {
    case BLOCK:
      rfh->BlockRequestsForFrame();
      break;
    case RESUME:
      rfh->ResumeBlockedRequestsForFrame();
      break;
    default:
      DCHECK_EQ(action, CANCEL);
      rfh->CancelBlockedRequestsForFrame();
      break;
  }
}
