void RenderViewHostImpl::DidCancelPopupMenu() {
  Send(new ViewMsg_SelectPopupMenuItem(GetRoutingID(), -1));
}
