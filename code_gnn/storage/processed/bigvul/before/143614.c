void RenderWidgetHostImpl::ExecuteEditCommand(const std::string& command,
                                              const std::string& value) {
  Send(new InputMsg_ExecuteEditCommand(GetRoutingID(), command, value));
}
