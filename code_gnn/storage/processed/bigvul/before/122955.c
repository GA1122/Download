void RenderWidgetHostImpl::ExecuteEditCommand(const std::string& command,
                                              const std::string& value) {
  Send(new ViewMsg_ExecuteEditCommand(GetRoutingID(), command, value));
}
