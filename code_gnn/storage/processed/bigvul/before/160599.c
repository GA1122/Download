void RenderFrameImpl::EnterFullscreen() {
  Send(new FrameHostMsg_ToggleFullscreen(routing_id_, true));
}
