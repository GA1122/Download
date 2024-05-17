void RenderFrameHostImpl::SetHasReceivedUserGesture() {
  Send(new FrameMsg_SetHasReceivedUserGesture(routing_id_));
}
