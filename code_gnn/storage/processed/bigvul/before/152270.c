void RenderFrameImpl::DidDisplayInsecureContent() {
  Send(new FrameHostMsg_DidDisplayInsecureContent(routing_id_));
}
