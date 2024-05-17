void RenderViewHostImpl::RequestFindMatchRects(int current_version) {
  Send(new ViewMsg_FindMatchRects(GetRoutingID(), current_version));
}
