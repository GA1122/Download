void RenderFrameImpl::ExitFullscreen() {
  Send(new FrameHostMsg_ToggleFullscreen(routing_id_, false));
}
