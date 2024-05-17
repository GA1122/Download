void RenderFrameImpl::AbortClientNavigation() {
  browser_side_navigation_pending_ = false;
  Send(new FrameHostMsg_AbortNavigation(routing_id_));
}
