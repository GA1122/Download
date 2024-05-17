void RenderViewHostImpl::ClearFocusedNode() {
  Send(new ViewMsg_ClearFocusedNode(GetRoutingID()));
}
