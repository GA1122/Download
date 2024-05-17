void RenderViewHostImpl::ReloadFrame() {
  Send(new ViewMsg_ReloadFrame(GetRoutingID()));
}
