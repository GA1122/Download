void WebContentsImpl::OnUserGesture() {
  FOR_EACH_OBSERVER(WebContentsObserver, observers_, DidGetUserGesture());

  ResourceDispatcherHostImpl* rdh = ResourceDispatcherHostImpl::Get();
  if (rdh)   
    rdh->OnUserGesture(this);
}
