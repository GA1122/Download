void RenderFrameImpl::dispatchLoad() {
  Send(new FrameHostMsg_DispatchLoad(routing_id_));
}
