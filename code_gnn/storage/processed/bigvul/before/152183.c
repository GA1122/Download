void RenderFrameImpl::AbortClientNavigation() {
  browser_side_navigation_pending_ = false;
  sync_navigation_callback_.Cancel();
  mhtml_body_loader_client_.reset();
  if (!IsPerNavigationMojoInterfaceEnabled())
    Send(new FrameHostMsg_AbortNavigation(routing_id_));
}
