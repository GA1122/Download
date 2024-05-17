void WebContentsImpl::SendUserGestureForResourceDispatchHost() {
  ResourceDispatcherHostImpl* rdh = ResourceDispatcherHostImpl::Get();

  if (rdh)
    rdh->OnUserGesture();
}
