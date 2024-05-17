void DevToolsClient::closeWindow() {
  Send(new DevToolsHostMsg_CloseWindow(routing_id()));
}
