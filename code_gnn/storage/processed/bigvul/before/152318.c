void RenderFrameImpl::FrameDidCallFocus() {
  Send(new FrameHostMsg_FrameDidCallFocus(routing_id_));
}
