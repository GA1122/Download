void RenderViewHostImpl::ExecuteCustomContextMenuCommand(
    int action, const CustomContextMenuContext& context) {
  Send(new ViewMsg_CustomContextMenuAction(GetRoutingID(), context, action));
}
