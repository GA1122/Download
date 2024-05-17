void RenderViewImpl::exitFullScreen() {
  Send(new ViewHostMsg_ToggleFullscreen(routing_id_, false));
}
