void RenderViewHostImpl::NotifyContextMenuClosed(
    const CustomContextMenuContext& context) {
  Send(new ViewMsg_ContextMenuClosed(GetRoutingID(), context));
}
