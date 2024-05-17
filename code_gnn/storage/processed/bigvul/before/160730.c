void RenderFrameImpl::SetHasReceivedUserGesture() {
  Send(new FrameHostMsg_SetHasReceivedUserGesture(routing_id_));
}
