void RenderFrameHostImpl::SaveImageAt(int x, int y) {
  Send(new FrameMsg_SaveImageAt(routing_id_, x, y));
}
