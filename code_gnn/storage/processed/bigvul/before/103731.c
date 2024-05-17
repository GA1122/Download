void DevToolsClient::requestDockWindow() {
  Send(new DevToolsHostMsg_RequestDockWindow(routing_id()));
}
