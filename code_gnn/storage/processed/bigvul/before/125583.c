void RenderViewHostImpl::DidCancelPopupMenu() {
  Send(new ViewMsg_SelectPopupMenuItems(GetRoutingID(), true,
                                        std::vector<int>()));
}
