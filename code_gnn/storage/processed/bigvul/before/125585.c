void RenderViewHostImpl::DidSelectPopupMenuItem(int selected_index) {
  Send(new ViewMsg_SelectPopupMenuItem(GetRoutingID(), selected_index));
}
