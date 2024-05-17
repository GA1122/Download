void RenderWidgetHostImpl::SetEditCommandsForNextKeyEvent(
    const std::vector<EditCommand>& commands) {
  Send(new InputMsg_SetEditCommandsForNextKeyEvent(GetRoutingID(), commands));
}
