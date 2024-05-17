void RenderFrameHostImpl::CopyImageAt(int x, int y) {
  Send(new FrameMsg_CopyImageAt(routing_id_, x, y));
}
