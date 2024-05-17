bool RenderFrameImpl::enterFullscreen() {
  Send(new FrameHostMsg_ToggleFullscreen(routing_id_, true));
  return true;
}
