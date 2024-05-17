bool RenderViewImpl::enterFullScreen() {
  Send(new ViewHostMsg_ToggleFullscreen(routing_id_, true));
  return true;
}
