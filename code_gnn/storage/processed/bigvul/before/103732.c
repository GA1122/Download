void DevToolsClient::requestUndockWindow() {
  Send(new DevToolsHostMsg_RequestUndockWindow(routing_id()));
}
