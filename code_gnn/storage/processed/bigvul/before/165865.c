void RenderFrameImpl::AbortClientNavigation() {
  browser_side_navigation_pending_ = false;
  sync_navigation_callback_.Cancel();
  if (!IsPerNavigationMojoInterfaceEnabled())
    Send(new FrameHostMsg_AbortNavigation(routing_id_));
}
