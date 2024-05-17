void RenderWidgetHostImpl::SetEditCommandsForNextKeyEvent(
    const std::vector<EditCommand>& commands) {
  Send(new ViewMsg_SetEditCommandsForNextKeyEvent(GetRoutingID(), commands));
}
