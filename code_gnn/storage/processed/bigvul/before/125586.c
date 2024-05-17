void RenderViewHostImpl::DidSelectPopupMenuItems(
    const std::vector<int>& selected_indices) {
  Send(new ViewMsg_SelectPopupMenuItems(GetRoutingID(), false,
                                        selected_indices));
}
