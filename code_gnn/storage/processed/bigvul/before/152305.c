void RenderFrameImpl::ExitFullscreen() {
  Send(new FrameHostMsg_ExitFullscreen(routing_id_));
}
