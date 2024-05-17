void RenderFrameHostImpl::Stop() {
  Send(new FrameMsg_Stop(routing_id_));
}
