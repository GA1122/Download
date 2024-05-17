void RenderFrameImpl::DispatchLoad() {
  Send(new FrameHostMsg_DispatchLoad(routing_id_));
}
