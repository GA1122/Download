bool RenderFrameImpl::exitFullscreen() {
  Send(new FrameHostMsg_ToggleFullscreen(routing_id_, false));
  return true;
}
