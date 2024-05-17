void RenderFrameImpl::DidContainInsecureFormAction() {
  Send(new FrameHostMsg_DidContainInsecureFormAction(routing_id_));
}
