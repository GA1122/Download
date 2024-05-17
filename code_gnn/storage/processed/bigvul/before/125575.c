void RenderViewHostImpl::CopyImageAt(int x, int y) {
  Send(new ViewMsg_CopyImageAt(GetRoutingID(), x, y));
}
