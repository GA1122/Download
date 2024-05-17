void RenderFrameHostImpl::NotifyUserActivation() {
  Send(new FrameMsg_NotifyUserActivation(routing_id_));
}
